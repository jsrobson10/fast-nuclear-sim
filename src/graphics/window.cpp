
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
#include "../parts.hpp"

using namespace sim::graphics;

static GLFWwindow* win;
static bool win_should_close = false;

static mesh MeshScene, MeshText;
static mesh MeshMon1, MeshMon2, MeshMon3;

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

	MeshScene.bind();
	MeshScene.load_model("../assets", "scene-baked.glb");

	glm::mat4 mat = glm::mat4(1);
	mat = glm::translate(mat, glm::vec3(-2.949, -1.7778 + 0.05, 3 - 0.05));
	mat = glm::rotate(mat, glm::radians<float>(-90), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians<float>(-90), glm::vec3(0, 1, 0));

	MeshText.model_matrix = mat;
	MeshText.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

	mat = glm::mat4(1);
	mat = glm::translate(mat, glm::vec3(-1.5 + 0.05, 3.949, 3 - 0.05));
	mat = glm::rotate(mat, glm::radians<float>(-90), glm::vec3(1, 0, 0));

	MeshMon1.model_matrix = mat;
	MeshMon1.colour_matrix = {
		1, 0.5, 0.5, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	mat = glm::translate(glm::mat4(1), glm::vec3(2.5, 0, 0)) * mat;
	
	MeshMon2.model_matrix = mat;
	MeshMon2.colour_matrix = {
		0.5, 1, 0.5, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	mat = glm::translate(glm::mat4(1), glm::vec3(2.5, 0, 0)) * mat;
	
	MeshMon3.model_matrix = mat;
	MeshMon3.colour_matrix = {
		0.5, 0.5, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

	glViewport(0, 0, 800, 600);
}

void window::loop()
{
	MeshText.bind(); MeshText.load_text("Reactor Core\n\nTODO", 0.1);
	MeshMon1.bind(); MeshMon1.load_text("Reactor Vessel\n\n", parts::vessel, 0.1);
	MeshMon2.bind(); MeshMon2.load_text("Steam Valve\n\n", parts::valve, 0.1);
	MeshMon3.bind(); MeshMon3.load_text("Coolant Pump\n\n", parts::pump, 0.1);

	glm::mat4 mat_projection = glm::perspective(glm::radians(80.0f), resize::get_aspect(), 0.01f, 20.f);
	glUniformMatrix4fv(shader::gl_projection, 1, false, &mat_projection[0][0]);

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	MeshScene.bind(); MeshScene.render();
	MeshText.bind(); MeshText.render();
	MeshMon1.bind(); MeshMon1.render();
	MeshMon2.bind(); MeshMon2.render();
	MeshMon3.bind(); MeshMon3.render();

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

