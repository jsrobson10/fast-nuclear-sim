
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
#include "equipment/pool_pump.hpp"
#include "../system.hpp"
#include "../util/streams.hpp"
#include "statebuffer.hpp"
#include "settings.hpp"
#include "ui.hpp"

using namespace Sim;
using namespace Sim::Graphics;
using namespace Sim::Graphics::Data;

static GLFWwindow* win;
static bool win_should_close = false;
static unsigned int ssbo_transforms;
static unsigned int ssbo_materials;
static unsigned int wait_at = 0;

static unsigned int main_rbo_colour;
static unsigned int main_rbo_depth;
static unsigned int main_fbo;

static int ssbo_transforms_at = 0;

static Mesh g_scene(true);
static std::vector<glm::mat4> g_scene_transforms;

static GLMesh gm_scene;
static GLMesh gm_dynamic_slow;
static GLMesh gm_player;

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
	mesh.set_baked(false);

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

void Window::reload_rbos()
{
	glm::vec<2, int> size = Resize::get_size();
	int msaa = Settings::get_msaa();

	glBindRenderbuffer(GL_RENDERBUFFER, main_rbo_colour);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_RGBA8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, main_rbo_colour);
	glBindRenderbuffer(GL_RENDERBUFFER, main_rbo_depth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH_COMPONENT, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, main_rbo_depth);
}

void Window::create()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_VISIBLE, false);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
	
	win = glfwCreateWindow(800, 600, "FastNuclearSim", nullptr, nullptr);
	glfwMakeContextCurrent(win);
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
		std::cout << "Info: Bindless textures are not supported. Using texture atlas instead.\n";
	}

	glfwSwapInterval(Settings::get_vsync() ? 1 : 0);

	glGenFramebuffers(1, &main_fbo);
	glGenRenderbuffers(1, &main_rbo_colour);
	glGenRenderbuffers(1, &main_rbo_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, main_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, main_rbo_colour);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, main_rbo_colour);
	glBindRenderbuffer(GL_RENDERBUFFER, main_rbo_depth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, main_rbo_depth);

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

	g_scene.add(model.load("control room"));
	g_scene.add(model.load("containment building"));
	g_scene.add(model.load("hallway"));

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
	equipment.push_back(new Equipment::PoolPump(model));

	Texture::generate_atlas();
	
	remesh_static();

	glfwShowWindow(win);
	Resize::check_fullscreen();

	// setup lighting and prerender shadows
	Shader::LIGHT.use();
	glUniform1f(Shader::LIGHT["far_plane"], 100.0f);
	GLLights::init();
	
	lights = std::make_unique<GLLights>(std::move(model.lights));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lights->ssbo);

	Shader::MAIN.use();
	glUniform1f(Shader::MAIN["far_plane"], 100.0f);
	glUniform1i(Shader::MAIN["shadows_enabled"], 1);

	glGenBuffers(1, &ssbo_transforms);
	glGenBuffers(1, &ssbo_materials);

	StateBuffer::init();
}

void update_slow()
{
	static Mesh mesh(true);
	mesh.clear();

	for(auto& monitor : monitors)
	{
		monitor->remesh_slow(mesh);
	}

	for(auto& equipment : equipment)
	{
		equipment->remesh_slow(mesh);
	}

	gm_dynamic_slow.bind();
	gm_dynamic_slow.set(mesh, GL_STREAM_DRAW);
}

void Window::reload()
{
	remesh_static();
}

void Window::update(double dt)
{
	static std::vector<glm::mat4> transforms;
	static std::vector<Data::Material> materials;

	transforms.clear();
	materials.clear();

	glfwPollEvents();

	for(auto& monitor : monitors)
	{
		monitor->update(dt);
		monitor->get_static_transforms(transforms);
		monitor->get_static_materials(materials);
	}

	for(auto& equipment : equipment)
	{
		equipment->update(dt);
		equipment->get_static_transforms(transforms);
		equipment->get_static_materials(materials);
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

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_transforms);
	glBufferData(GL_SHADER_STORAGE_BUFFER, transforms.size() * sizeof(transforms[0]), transforms.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_materials);
	glBufferData(GL_SHADER_STORAGE_BUFFER, materials.size() * sizeof(materials[0]), materials.data(), GL_STREAM_DRAW);

	if(wait_at++ % Settings::get_text_refreshes() == 0)
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_transforms);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssbo_materials);
}

void Window::render_scene()
{
	gm_scene.bind();
	gm_scene.render();
	
	Focus::render();
}

void Window::render_dynamic()
{
	gm_dynamic_slow.bind();
	gm_dynamic_slow.render();
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

	StateBuffer::set({mat_camera, mat_projection});
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &camera_pos[0]);
	projection_matrix = mat_projection;
	
	glBindFramebuffer(GL_FRAMEBUFFER, main_fbo);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size.x, size.y);

	bind_scene_ssbo();
	render_scene();
	render_dynamic();
	monitor_cctv->render_screen();

	glUniform3f(Shader::MAIN["brightness"], 1, 1, 1);
	UI::render();
	Focus::render_ui();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, main_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
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

