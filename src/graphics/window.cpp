
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
#include "../system.hpp"
#include "ui.hpp"

using namespace Sim::Graphics;

static GLFWwindow* win;
static bool win_should_close = false;

static GLMesh mesh_scene;
static Monitor::Vessel monitor_vessel;
static Monitor::Core monitor_core;
static Monitor::PrimaryLoop monitor_primary_loop;
static Monitor::SecondaryLoop monitor_secondary_loop;
static Monitor::Turbine monitor_turbine;

glm::mat4 Window::projection_matrix;

void GLAPIENTRY cb_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
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

	Shader::init_program();

	Sim::System& sys = Sim::System::active;
	Mesh m, m2;

	m.load_model("../assets", "scene-baked.glb");

	// find the floor parts of the model and set them slightly transparent
	for(int i = 0; i < m.indices.size(); i += 3)
	{
		Arrays::Vertex& v1 = m.vertices[m.indices[i]];
		Arrays::Vertex& v2 = m.vertices[m.indices[i + 1]];
		Arrays::Vertex& v3 = m.vertices[m.indices[i + 2]];

		if(v1.pos.z <= 0 && v2.pos.z <= 0 && v3.pos.z <= 0)
		{
			v1.colour.w = 0.95;
			v2.colour.w = 0.95;
			v3.colour.w = 0.95;
		}
	}

	m2.load_model("../assets/model", "monitor_graphics.stl");
	m.add(m2, glm::mat4(1));

	mesh_scene.bind();
	mesh_scene.set(m, GL_STATIC_DRAW);

	monitor_core.init();
	monitor_vessel.init();
	monitor_primary_loop.init();
	monitor_secondary_loop.init();
	monitor_turbine.init();

	glfwShowWindow(win);
	glViewport(0, 0, 800, 600);
}

void Window::update(double dt)
{
	glfwPollEvents();

	monitor_core.update(dt);
	monitor_vessel.update(dt);
	monitor_primary_loop.update(dt);
	monitor_secondary_loop.update(dt);
	monitor_turbine.update(dt);

	UI::update(dt);
}

void render_scene()
{
	mesh_scene.bind();
	mesh_scene.uniform();
	mesh_scene.render();

	monitor_core.render();
	monitor_vessel.render();
	monitor_primary_loop.render();
	monitor_secondary_loop.render();
	monitor_turbine.render();

	Focus::render();
}

void Window::render()
{
	glm::mat4 mat_camera = Camera::get_matrix();
	mat_camera = glm::scale(mat_camera, {1, 1, -1});

	glm::vec3 brightness = glm::vec3(Sim::System::active.grid->get_light_intensity());
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), Resize::get_aspect(), 0.01f, 20.f);
	glUniformMatrix4fv(Shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(Shader::gl_camera, 1, false, &mat_camera[0][0]);
	glUniform3fv(Shader::gl_brightness, 1, &brightness[0]);
	projection_matrix = mat_projection;

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);
	
	render_scene();

	mat_camera = Camera::get_matrix();
	glUniformMatrix4fv(Shader::gl_camera, 1, false, &mat_camera[0][0]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);

	render_scene();

	brightness = glm::vec3(1);
	glUniform3fv(Shader::gl_brightness, 1, &brightness[0]);

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

