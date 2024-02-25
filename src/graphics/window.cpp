
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#include <iostream>

#include "mesh/mesh.hpp"
#include "mesh/arrays.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "input/focus.hpp"
#include "camera.hpp"
#include "resize.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "mesh/font.hpp"
#include "locations.hpp"
#include "monitor/vessel.hpp"
#include "monitor/core.hpp"
#include "monitor/primary_loop.hpp"
#include "monitor/secondary_loop.hpp"
#include "monitor/turbine.hpp"
#include "mesh/texture.hpp"
#include "mesh/gllight.hpp"
#include "../system.hpp"
#include "../util/streams.hpp"
#include "ui.hpp"

using namespace Sim::Graphics;

static GLFWwindow* win;
static bool win_should_close = false;
static unsigned int ssbo_lights;
static unsigned int ssbo_shadow_maps;
static double secs_wait_at = 0;
static double secs_wait_now = 0;

static int gm_dynamic_slow_at = 0;

static GLMesh gm_scene;
static GLMesh gm_dynamic_slow[2];
static GLMesh gm_dynamic_fast;
static Mesh m_dynamic_fast;

static std::vector<GLLight> lights;

static Monitor::Vessel monitor_vessel;
static Monitor::Core monitor_core;
static Monitor::PrimaryLoop monitor_primary_loop;
static Monitor::SecondaryLoop monitor_secondary_loop;
static Monitor::Turbine monitor_turbine;

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

void Window::create()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	if(!glGetTextureHandleARB || !glMakeTextureHandleResidentARB)
	{
		std::cerr << "Fatal: Bindless textures not supported\n";

		if(!glGetTextureHandleARB)
			std::cerr << "  Missing: glGetTextureHandleARB\n";
		if(!glMakeTextureHandleResidentARB)
			std::cerr << "  Missing: glMakeTextureHandleResidentARB\n";

		close();
		return;
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
	Camera::init();
	Font::init();
	UI::init();

	Shader::MAIN.load("../assets/shader", "main.vsh", "main.fsh");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Sim::System& sys = *System::active;
	Mesh m_scene;

	m_scene.load_model("../assets", "scene.glb");

	m_scene.lights[0].pos.x = 6;
	m_scene.lights[1].pos.x = 0;

	// find the floor parts of the model and set them slightly transparent
	for(int i = 0; i < m_scene.indices.size(); i += 3)
	{
		Arrays::Vertex& v1 = m_scene.vertices[m_scene.indices[i]];
		Arrays::Vertex& v2 = m_scene.vertices[m_scene.indices[i + 1]];
		Arrays::Vertex& v3 = m_scene.vertices[m_scene.indices[i + 2]];

		if(v1.pos.z <= 0 && v2.pos.z <= 0 && v3.pos.z <= 0)
		{
			v1.colour.w = 0.95;
			v2.colour.w = 0.95;
			v3.colour.w = 0.95;
		}
	}

	// send all the light data
	glGenBuffers(1, &ssbo_lights);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_lights);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_scene.lights.size() * sizeof(m_scene.lights[0]), &m_scene.lights[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_lights);

	glUniform1i(Shader::MAIN["lights_count"], m_scene.lights.size());

	monitor_core.init(m_scene);
	monitor_vessel.init(m_scene);
	monitor_primary_loop.init(m_scene);
	monitor_secondary_loop.init(m_scene);
	monitor_turbine.init(m_scene);

	gm_scene.bind();
	gm_scene.set(m_scene, GL_STATIC_DRAW);

	glfwShowWindow(win);
/*	
	// setup lighting and prerender shadows
	Shader::LIGHT.load("../assets/shader", "light.vsh", "light.gsh", "light.fsh");
	glUniform1f(Shader::LIGHT["far_plane"], 100.0f);
	GLLight::init();
	
	std::vector<unsigned long> light_handles;
	
	for(int i = 0; i < m_scene.lights.size(); i++)
	{
		GLLight light(m_scene.lights[i]);
		light.render();

		light_handles.push_back(light.handle);
		lights.push_back(std::move(light));
	}

	for(int i = 0; i < lights.size(); i++)
	{
		std::cout << "handle " << i << ": " << light_handles[i] << "\n";
	}

	Shader::MAIN.use();*/
	glUniform1f(Shader::MAIN["far_plane"], 100.0f);
	
/*	// send all the light shadow map handles
	glGenBuffers(1, &ssbo_shadow_maps);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_shadow_maps);
	glBufferData(GL_SHADER_STORAGE_BUFFER, light_handles.size() * sizeof(light_handles[0]), &light_handles[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_shadow_maps);*/
}

void update_slow()
{
	Mesh mesh;

	monitor_core.remesh_slow(mesh);
	monitor_vessel.remesh_slow(mesh);
	monitor_primary_loop.remesh_slow(mesh);
	monitor_secondary_loop.remesh_slow(mesh);
	monitor_turbine.remesh_slow(mesh);

	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].set(mesh, GL_DYNAMIC_DRAW);
	gm_dynamic_slow_at = (gm_dynamic_slow_at + 1) % 2;

	UI::update_slow();
}

void Window::update(double dt)
{
	Mesh mesh;

	glfwPollEvents();

	monitor_core.update(dt);
	monitor_vessel.update(dt);
	monitor_primary_loop.update(dt);
	monitor_secondary_loop.update(dt);
	monitor_turbine.update(dt);
	
	UI::update(dt);
	
	monitor_core.remesh_fast(mesh);
	monitor_vessel.remesh_fast(mesh);
	monitor_primary_loop.remesh_fast(mesh);
	monitor_secondary_loop.remesh_fast(mesh);
	monitor_turbine.remesh_fast(mesh);
	
	if(mesh != m_dynamic_fast)
	{
		gm_dynamic_fast.bind();
		gm_dynamic_fast.set(mesh, GL_DYNAMIC_DRAW);
		m_dynamic_fast = std::move(mesh);
	}

	secs_wait_now += dt;
	if(secs_wait_now > secs_wait_at + 1.0/30.0)
	{
		secs_wait_at += 1.0/30.0;
		update_slow();
	}
}

void Window::render_scene()
{
	gm_scene.bind();
	gm_scene.uniform();
	gm_scene.render();

	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].uniform();
	gm_dynamic_slow[gm_dynamic_slow_at].render();

	gm_dynamic_fast.bind();
	gm_dynamic_fast.uniform();
	gm_dynamic_fast.render();

	monitor_core.render();
	monitor_vessel.render();
	monitor_primary_loop.render();
	monitor_secondary_loop.render();
	monitor_turbine.render();

	Focus::render();
}

void Window::render()
{
	glm::vec<2, int> size = Resize::get_size();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, size.x, size.y);

	glm::vec3 camera_pos = Camera::get_pos();
	glm::mat4 mat_camera = Camera::get_matrix();
	mat_camera = glm::scale(mat_camera, {1, 1, -1});
	camera_pos.z *= -1;

	Shader::MAIN.use();

	glm::vec3 brightness = glm::vec3(System::active->grid.get_light_intensity());
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), Resize::get_aspect(), 0.01f, 20.f);
	glUniformMatrix4fv(Shader::MAIN["projection"], 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &mat_camera[0][0]);
	glUniform3fv(Shader::MAIN["brightness"], 1, &brightness[0]);
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &camera_pos[0]);
	projection_matrix = mat_projection;

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	
	render_scene();

	camera_pos.z *= -1;
	mat_camera = Camera::get_matrix();
	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &mat_camera[0][0]);
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &camera_pos[0]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);

	render_scene();

	brightness = glm::vec3(1);
	glUniform3fv(Shader::MAIN["brightness"], 1, &brightness[0]);

	UI::render();
	Focus::render_ui();

	glfwSwapBuffers(win);
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

