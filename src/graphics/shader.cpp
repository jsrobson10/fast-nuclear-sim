
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.hpp"
#include "window.hpp"

using namespace sim::graphics;

static unsigned int prog_id;

int shader::gl_tex_mat;
int shader::gl_model;
int shader::gl_camera;
int shader::gl_projection;

static int load_shader(const char* src, int type)
{
	int id = glCreateShader(type);

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return id;
}

static std::string read_shader(const char* path)
{
	std::stringstream ss;
	std::ifstream file(path, std::ios::binary);
	char buff[1024];

	while(!file.eof())
	{
		file.read(buff, 1024);
		ss.write(buff, file.gcount());
	}

	return ss.str();
}

unsigned int shader::init_program()
{
	std::string shader_vsh = read_shader("../assets/shader/main.vsh");
	std::string shader_fsh = read_shader("../assets/shader/main.fsh");
	
	int success;
	int vsh_id = load_shader(shader_vsh.c_str(), GL_VERTEX_SHADER);
	int fsh_id = load_shader(shader_fsh.c_str(), GL_FRAGMENT_SHADER);
	prog_id = glCreateProgram();

	glAttachShader(prog_id, vsh_id);
	glAttachShader(prog_id, fsh_id);
	glLinkProgram(prog_id);
	glGetProgramiv(prog_id, GL_LINK_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
		std::cout << "Shader Link Error: " << infoLog << std::endl;
		window::close();
		return 0;
	}
	
	gl_tex_mat = glGetUniformLocation(prog_id, "tex_mat");
	gl_model = glGetUniformLocation(prog_id, "model");
	gl_camera = glGetUniformLocation(prog_id, "camera");
	gl_projection = glGetUniformLocation(prog_id, "projection");

	glUseProgram(prog_id);
	glDeleteShader(vsh_id);
	glDeleteShader(fsh_id);

	return prog_id;
}

