
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
Shader Shader::LIGHT;
Shader* Shader::ACTIVE;

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

static std::string read_shader(const char* base, const char* file)
{
	std::string path = std::string(base) + "/" + std::string(file);
	return read_shader(path.c_str());
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
	load(path, file_vsh, nullptr, file_fsh);
}

void Shader::load(const char* path, const char* file_vsh, const char* file_gsh, const char* file_fsh)
{
	std::string shader_vsh = file_vsh ? read_shader(path, file_vsh) : "";
	std::string shader_gsh = file_gsh ? read_shader(path, file_gsh) : "";
	std::string shader_fsh = file_fsh ? read_shader(path, file_fsh) : "";

	int success;
	int vsh_id = file_vsh ? load_shader(shader_vsh.c_str(), GL_VERTEX_SHADER) : 0;
	int gsh_id = file_gsh ? load_shader(shader_gsh.c_str(), GL_GEOMETRY_SHADER) : 0;
	int fsh_id = file_fsh ? load_shader(shader_fsh.c_str(), GL_FRAGMENT_SHADER) : 0;
	
	prog_id = glCreateProgram();

	if(file_vsh)
		glAttachShader(prog_id, vsh_id);
	if(file_gsh)
		glAttachShader(prog_id, gsh_id);
	if(file_fsh)
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

	if(file_vsh)
		glDeleteShader(vsh_id);
	if(file_gsh)
		glDeleteShader(gsh_id);
	if(file_fsh)
		glDeleteShader(fsh_id);

	ACTIVE = this;
}

void Shader::use()
{
	glUseProgram(prog_id);
	ACTIVE = this;
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

unsigned int Shader::get(const char* pos)
{
	return operator[](pos);
}

