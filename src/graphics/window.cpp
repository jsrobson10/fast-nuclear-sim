
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
#include "camera.hpp"
#include "resize.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "mesh/font.hpp"
#include "locations.hpp"
#include "monitor/vessel.hpp"
#include "monitor/core.hpp"
#include "mesh/texture.hpp"
#include "ui.hpp"

using namespace sim::graphics;

static GLFWwindow* win;
static bool win_should_close = false;

static glmesh MeshScene;
static monitor::vessel MonitorVessel;
static monitor::core MonitorCore;

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
	font::init();
	ui::init();

	shader::init_program();

	sim::system& sys = sim::system::active;
	mesh m;

	m.load_model("../assets", "scene-baked.glb");
	MeshScene.bind();
	MeshScene.set(m, GL_STATIC_DRAW);

	sys.scene.load_model("../assets/model", "scene_collisions.stl");
//	MeshCollisionScene.bind();
//	MeshCollisionScene.set(sys.scene.to_lines(), GL_STATIC_DRAW);

	MonitorCore.init();
	MonitorVessel.init();

	glfwShowWindow(win);
	glViewport(0, 0, 800, 600);
}

void window::update(double dt)
{
	glfwPollEvents();

	MonitorCore.update();
	MonitorVessel.update();

	ui::update(dt);
}

void window::render()
{
	glm::mat4 mat_camera = camera::get_matrix();
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), resize::get_aspect(), 0.01f, 20.f);
	glUniformMatrix4fv(shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(shader::gl_camera, 1, false, &mat_camera[0][0]);

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	MeshScene.bind();
	MeshScene.uniform();
	MeshScene.render();

	MonitorCore.render();
	MonitorVessel.render();

	ui::render();

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

