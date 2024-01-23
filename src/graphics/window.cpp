
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(cb_debug_message, nullptr);

	keyboard::init();
	mouse::init();
	resize::init();
	font::init();

	shader::init_program();

//	Model.load("Minimalistic Modern Office", "Minimalistic Modern Office.fbx");
	Model.load("../assets", "scene.obj");

	glViewport(0, 0, 800, 600);
}

void window::loop()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mat_colour = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	camera::update();

	glm::mat4 mat_model = camera::get_model_matrix();

	double mouse_x, mouse_y;
	mouse::get(mouse_x, mouse_y);
	
	glm::mat4 mat_projection = glm::perspective(glm::radians(90.0f), (float)resize::get_aspect(), 0.01f, 20.f);

	glUniformMatrix4fv(shader::gl_tex_mat, 1, false, &mat_colour[0][0]);
	glUniformMatrix4fv(shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(shader::gl_model, 1, false, &mat_model[0][0]);
	glUniform1i(shader::gl_do_tex, 0);
	
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

