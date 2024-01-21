
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#include <iostream>

#include "model.hpp"
#include "arrays.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "camera.hpp"
#include "resize.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "font.hpp"

using namespace sim::graphics;

static GLFWwindow* win;
static bool win_should_close = false;
static model Model;

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(cb_debug_message, nullptr);

	keyboard::init();
	mouse::init();
	resize::init();
	font::init();

	shader::init_program();

	Model.alloc();
	Model.load("monkey.obj");

	glViewport(0, 0, 800, 600);
}

void window::loop()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mat_colour = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

	camera::update();

	glm::mat4 mat_model = camera::get();

	double mouse_x, mouse_y;
	mouse::get(mouse_x, mouse_y);
	
	mat_model = glm::translate(mat_model, glm::vec3(0.0f, 0.0f, -5.0f));
	mat_model = glm::rotate(mat_model, float(M_PI * 0.125), glm::vec3(1, 1, 1));
	
	glm::mat4 mat_projection = glm::perspective(float(M_PI * 0.25), (float)resize::get_aspect(), 0.1f, 100.f);

	glUniformMatrix4fv(shader::gl_tex_mat, 1, false, &mat_colour[0][0]);
	glUniformMatrix4fv(shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(shader::gl_model, 1, false, &mat_model[0][0]);
	glUniform1i(shader::gl_do_tex, 0);
	
	Model.bind();
	Model.render();

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

