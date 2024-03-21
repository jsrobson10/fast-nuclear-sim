
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#include <iostream>
#include <vector>
#include <memory>

#include "data/mesh.hpp"
#include "data/arrays.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "input/focus.hpp"
#include "camera.hpp"
#include "resize.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "data/font.hpp"
#include "monitor/vessel.hpp"
#include "monitor/core.hpp"
#include "monitor/primary_loop.hpp"
#include "monitor/secondary_loop.hpp"
#include "monitor/turbine.hpp"
#include "monitor/cctv.hpp"
#include "data/texture.hpp"
#include "data/model.hpp"
#include "data/gllights.hpp"
#include "data/meshgen.hpp"
#include "data/material.hpp"
#include "equipment/reactor.hpp"
#include "equipment/generator.hpp"
#include "equipment/pool.hpp"
#include "../system.hpp"
#include "../util/streams.hpp"
#include "ui.hpp"

using namespace Sim;
using namespace Sim::Graphics;
using namespace Sim::Graphics::Data;

constexpr int SSBO_TRANSFORMS_LEN = 2;

static GLFWwindow* win;
static bool win_should_close = false;
static unsigned int ssbo_transforms[SSBO_TRANSFORMS_LEN];
static unsigned int wait_at = 0;

static int gm_dynamic_slow_at = 0;
static int ssbo_transforms_at = 0;

static Mesh g_scene;
static std::vector<glm::mat4> g_scene_transforms;

static GLMesh gm_scene;
static GLMesh gm_player;
static GLMesh gm_dynamic_slow[2];

static std::unique_ptr<GLLights> lights;
static std::vector<MeshGen*> monitors;
static std::vector<MeshGen*> equipment;

static Monitor::CCTV* monitor_cctv;

glm::mat4 Window::projection_matrix;

static void GLAPIENTRY cb_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(severity == GL_DEBUG_SEVERITY_HIGH)
	{
		throw std::runtime_error(message);
	}

	else if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		std::cout << "GL CALLBACK: " << message << "\n";
	}
}

void remesh_static()
{
	Mesh mesh(g_scene);

	for(auto& monitor : monitors)
	{
		monitor->remesh_static(mesh);
	}

	for(auto& equipment : equipment)
	{
		equipment->remesh_static(mesh);
	}

	gm_scene.bind();
	gm_scene.set(mesh, GL_STATIC_DRAW);
	g_scene_transforms = std::move(mesh.transforms);

	std::cout << "Total triangle count: " << mesh.indices.size() / 3 << "\n";
}

void Window::create()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_VISIBLE, false);
	glfwWindowHint(GLFW_SAMPLES, 16);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
	
	win = glfwCreateWindow(800, 600, "FastNuclearSim", nullptr, nullptr);

	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);

	GLenum err = glewInit();

	if(err != GLEW_OK)
	{
		std::cerr << "GLEW Init Failed: " << glewGetErrorString(err) << "\n";
		close();
		return;
	}

	if(glGetTextureHandleARB && glMakeTextureHandleResidentARB)
	{
		Shader::add_define("USE_BINDLESS_TEXTURES");
		Shader::USE_BINDLESS_TEXTURES = true;
	}

	else
	{
		std::cout << "Warning: Bindless textures are not supported. Using texture atlas instead.\n";
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(cb_debug_message, nullptr);

	Keyboard::init();
	Mouse::init();
	Resize::init();
	Texture::init();
	Fonts::init();
	UI::init();
	Shader::init();

	Shader::MAIN.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Sim::System& sys = *System::active;

	Model model("../assets", "scene.glb");
	Mesh m_player = model.load("visual_player");

	gm_player.bind();
	gm_player.bind_ssbo();
	gm_player.set(m_player, GL_STATIC_DRAW);

	g_scene.add(model.load("cr"));
	g_scene.add(model.load("cb"));
	g_scene.add(model.load("hw"));
	g_scene.bake_transforms();

	Camera::init(model);

	glUniform1i(Shader::MAIN["lights_count"], model.lights.size());

	monitors.push_back(new Monitor::Core(model));
	monitors.push_back(new Monitor::Vessel(model));
	monitors.push_back(new Monitor::PrimaryLoop(model));
	monitors.push_back(new Monitor::SecondaryLoop(model));
	monitors.push_back(new Monitor::Turbine(model));
	monitors.push_back(monitor_cctv = new Monitor::CCTV(model));

	equipment.push_back(new Equipment::Reactor(model));
	equipment.push_back(new Equipment::Generator(model));
	equipment.push_back(new Equipment::Pool(model));

	Texture::generate_atlas();
	
	remesh_static();

	glfwShowWindow(win);

	// setup lighting and prerender shadows
	Shader::LIGHT.use();
	glUniform1f(Shader::LIGHT["far_plane"], 100.0f);
	GLLights::init();
	
	lights = std::make_unique<GLLights>(std::move(model.lights));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lights->ssbo);

	Shader::MAIN.use();
	glUniform1f(Shader::MAIN["far_plane"], 100.0f);
	glUniform1i(Shader::MAIN["shadows_enabled"], 1);

	// setup the transforms ssbos and their initial values
	
	std::vector<glm::mat4> transforms;

	for(auto& monitor : monitors)
	{
		monitor->get_static_transforms(transforms);
	}

	for(auto& equipment : equipment)
	{
		equipment->get_static_transforms(transforms);
	}

	glGenBuffers(SSBO_TRANSFORMS_LEN, ssbo_transforms);

	for(int i = 0; i < SSBO_TRANSFORMS_LEN; i++)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_transforms[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, transforms.size() * sizeof(transforms[0]), &transforms[0], GL_DYNAMIC_DRAW);
	}
}

void update_slow()
{
	Mesh mesh;

	for(auto& monitor : monitors)
	{
		monitor->remesh_slow(mesh);
	}

	for(auto& equipment : equipment)
	{
		equipment->remesh_slow(mesh);
	}

	mesh.bake_transforms();

	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].set(mesh, GL_DYNAMIC_DRAW);
	gm_dynamic_slow_at = (gm_dynamic_slow_at + 1) % 2;

	UI::update_slow();
}

void Window::reload()
{
	remesh_static();
}

void Window::update(double dt)
{
	Mesh mesh;
	std::vector<glm::mat4> transforms;

	glfwPollEvents();

	for(auto& monitor : monitors)
	{
		monitor->update(dt);
		monitor->get_static_transforms(transforms);
	}

	for(auto& equipment : equipment)
	{
		equipment->update(dt);
		equipment->get_static_transforms(transforms);
	}

	for(int i = 0; i < transforms.size(); i++)
	{
		transforms[i] = g_scene_transforms[i] * transforms[i];
	}

	if(transforms.size() != g_scene_transforms.size())
	{
		std::cerr << "Transforms size mismatch! " << transforms.size() << " != " << g_scene_transforms.size() << "\n";
		close();
	}
	
	UI::update(dt);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_transforms[ssbo_transforms_at]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, transforms.size() * sizeof(transforms[0]), &transforms[0], GL_STREAM_DRAW);
	ssbo_transforms_at = (ssbo_transforms_at + 1) % SSBO_TRANSFORMS_LEN;

	if(wait_at++ % 4 == 0)
	{
		update_slow();
	}
}

void Window::render_player()
{
	gm_player.bind();
	gm_player.bind_ssbo();
	gm_player.render();
}

void Window::bind_scene_ssbo()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_transforms[ssbo_transforms_at]);
}

void Window::render_scene()
{
	gm_scene.bind();
	gm_scene.render();
	
	Focus::render();
}

void Window::render_dynamic()
{
	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].render();
}

void Window::render()
{
	glFrontFace(GL_CCW);

	glm::vec<2, int> size = Resize::get_size();
	glm::vec3 camera_pos = Camera::get_pos();
	glm::mat4 mat_camera = Camera::get_matrix();

	Shader::MAIN.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, lights->texid);
	glUniform1i(Shader::MAIN["shadow_maps"], 0);
	
	glm::vec3 brightness = glm::vec3(System::active->grid.get_light_intensity());
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), Resize::get_aspect(), 0.01f, 100.f);
	glUniform3fv(Shader::MAIN["brightness"], 1, &brightness[0]);

	glm::mat4 mat_player = glm::mat4(1);
	mat_player = glm::translate(mat_player, glm::vec3(Camera::get_pos_base()));
	mat_player = glm::rotate(mat_player, (float)glm::radians(90 - Camera::get_yaw()), glm::vec3(0, 0, 1));

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gm_player.ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), &mat_player, GL_STREAM_DRAW);
	
	monitor_cctv->render_view();

	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &mat_camera[0][0]);
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &camera_pos[0]);
	glUniformMatrix4fv(Shader::MAIN["projection"], 1, false, &mat_projection[0][0]);
	projection_matrix = mat_projection;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size.x, size.y);

	bind_scene_ssbo();
	render_scene();
	render_dynamic();
	monitor_cctv->render_screen();

	brightness = glm::vec3(1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUniform3fv(Shader::MAIN["brightness"], 1, &brightness[0]);

	UI::render();
	Focus::render_ui();

	glfwSwapBuffers(win);
	
	Shader::LIGHT.use();
	glFrontFace(GL_CW);
	lights->render();
}

bool Window::should_close()
{
	return win_should_close || glfwWindowShouldClose(win);
}

void Window::close()
{
	win_should_close = true;
}

void Window::destroy()
{
	glfwDestroyWindow(win);
	glfwTerminate();
}

GLFWwindow* Window::get_window()
{
	return win;
}

