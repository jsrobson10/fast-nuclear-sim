
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "arrays.hpp"
#include "keyboard.hpp"
#include "resize.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "font.hpp"

using namespace sim::graphics;

static GLFWwindow* win;
static bool win_should_close = false;

void GLAPIENTRY cb_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "GL CALLBACK: " << message << "\n";
}

void window::create()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	win = glfwCreateWindow(800, 600, "FastNuclearSim", nullptr, nullptr);

	glfwMakeContextCurrent(win);
	
	GLenum err = glewInit();
	
	if(err != GLEW_OK)
	{
		std::cout << "GLEW Init Failed: " << glewGetErrorString(err) << "\n";
		close();
		return;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(cb_debug_message, nullptr);

	keyboard::init();
	resize::init();
	font::init();

	shader::init_program();
	arrays::init();

	glViewport(0, 0, 800, 600);
}

void window::loop()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(win);
	glfwPollEvents();
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
	glfwTerminate();
}

GLFWwindow* window::get_window()
{
	return win;
}

