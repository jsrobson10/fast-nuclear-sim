
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.hpp"
#include "window.hpp"

using namespace Sim::Graphics;

Shader Shader::MAIN;
Shader Shader::BLUR;

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

Shader::Shader()
{

}

Shader::Shader(Shader&& o)
{
	prog_id = o.prog_id;
	o.prog_id = 0;
}

Shader::~Shader()
{
	if(prog_id)
	{
		glDeleteProgram(prog_id);
	}
}

void Shader::load(const char* path, const char* file_vsh, const char* file_fsh)
{
	std::string path_vsh = std::string(path) + "/" + std::string(file_vsh);
	std::string path_fsh = std::string(path) + "/" + std::string(file_fsh);
	std::string shader_vsh = read_shader(path_vsh.c_str());
	std::string shader_fsh = read_shader(path_fsh.c_str());

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
		std::cout << "Shader Link Error (" << path << "," << file_vsh << "," << file_fsh << "): " << infoLog << std::endl;
		Window::close();
		return;
	}

	glUseProgram(prog_id);
	glDeleteShader(vsh_id);
	glDeleteShader(fsh_id);
}

void Shader::use()
{
	glUseProgram(prog_id);
}

unsigned int Shader::operator [](const char* pos)
{
	auto it = uniform_locations.find(pos);

	if(it != uniform_locations.end())
	{
		return it->second;
	}

	return uniform_locations[pos] = glGetUniformLocation(prog_id, pos);
}

