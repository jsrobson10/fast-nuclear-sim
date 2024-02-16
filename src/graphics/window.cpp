
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
#include "ui.hpp"

using namespace sim::graphics;

static GLFWwindow* win;
static bool win_should_close = false;

static glmesh mesh_scene;
static monitor::vessel monitor_vessel;
static monitor::core monitor_core;
static monitor::primary_loop monitor_primary_loop;
static monitor::secondary_loop monitor_secondary_loop;
static monitor::turbine monitor_turbine;

glm::mat4 window::projection_matrix;

void GLAPIENTRY cb_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		std::cout << "GL CALLBACK: " << message << "\n";
	}
}

void window::create()
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

	keyboard::init();
	mouse::init();
	resize::init();
	texture::init();
	camera::init();
	font::init();
	ui::init();

	shader::init_program();

	sim::system& sys = sim::system::active;
	mesh m, m2;

	m.load_model("../assets", "scene-baked.glb");
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

void window::update(double dt)
{
	glfwPollEvents();

	monitor_core.update(dt);
	monitor_vessel.update(dt);
	monitor_primary_loop.update(dt);
	monitor_secondary_loop.update(dt);
	monitor_turbine.update(dt);

	ui::update(dt);
}

void window::render()
{
	glm::mat4 mat_camera = camera::get_matrix();
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), resize::get_aspect(), 0.01f, 20.f);
	glUniformMatrix4fv(shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(shader::gl_camera, 1, false, &mat_camera[0][0]);
	projection_matrix = mat_projection;

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mesh_scene.bind();
	mesh_scene.uniform();
	mesh_scene.render();

	monitor_core.render();
	monitor_vessel.render();
	monitor_primary_loop.render();
	monitor_secondary_loop.render();
	monitor_turbine.render();

	focus::render();
	ui::render();
	focus::render_ui();

	glfwSwapBuffers(win);
}

bool window::should_close()
{
	return win_should_close || glfwWindowShouldClose(win);
}

void window::close()
{
	win_should_close = true;
}

void window::destroy()
{
	glfwDestroyWindow(win);
	glfwTerminate();
}

GLFWwindow* window::get_window()
{
	return win;
}

