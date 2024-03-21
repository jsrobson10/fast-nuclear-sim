
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

#include "shader.hpp"
#include "shadersource.hpp"
#include "window.hpp"

using namespace Sim::Graphics;

Shader Shader::MAIN;
Shader Shader::LIGHT;
Shader Shader::CCTV;

Shader* Shader::ACTIVE;

bool Shader::USE_BINDLESS_TEXTURES = false;

static std::vector<std::string> shader_compiler_flags;

void Shader::add_define(const std::string& flag)
{
	shader_compiler_flags.push_back(flag);
}

void Shader::init()
{
	Shader::Source sources_main[] = {
		{ShaderSource::MAIN_VSH, "main.vsh", GL_VERTEX_SHADER},
		{ShaderSource::MAIN_FSH, "main.fsh", GL_FRAGMENT_SHADER}
	};

	Shader::Source sources_light[] = {
		{ShaderSource::LIGHT_VSH, "light.vsh", GL_VERTEX_SHADER},
		{ShaderSource::LIGHT_GSH, "light.gsh", GL_GEOMETRY_SHADER},
		{ShaderSource::LIGHT_FSH, "light.fsh", GL_FRAGMENT_SHADER}
	};

	Shader::Source sources_cctv[] = {
		{ShaderSource::CCTV_VSH, "cctv.vsh", GL_VERTEX_SHADER},
		{ShaderSource::CCTV_FSH, "cctv.fsh", GL_FRAGMENT_SHADER}
	};

	Shader::MAIN.load(sources_main, "main", 2);
	Shader::LIGHT.load(sources_light, "light", 3);
	Shader::CCTV.load(sources_cctv, "cctv", 2);
}

std::string apply_shader_compiler_flags(const char* source)
{
	std::stringstream ss;
	ss << "#version 430 core\n";

	for(const auto& flag : shader_compiler_flags)
	{
		ss << "#define " << flag << "\n";
	}

	ss << source;
	return ss.str();
}

Shader::Source::Source(const char* data, const char* name, GLenum type)
{
	int success;
	std::string source = apply_shader_compiler_flags(data);
	data = source.c_str();

	id = glCreateShader(type);
	glShaderSource(id, 1, &data, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::string entry = std::format("Shader Compile Error ({0}): {1}", name, infoLog);
		throw std::runtime_error(entry);
	}
}

Shader::Source::Source(Source&& o)
{
	id = o.id;
	o.id = 0;
}

Shader::Source::~Source()
{
	if(id) glDeleteShader(id);
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

void Shader::load(const Source* sources, const char* name, int count)
{
	int success;
	prog_id = glCreateProgram();

	for(int i = 0; i < count; i++)
	{
		glAttachShader(prog_id, sources[i].id);
	}

	glLinkProgram(prog_id);
	glGetProgramiv(prog_id, GL_LINK_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
		std::string entry = std::format("Shader Link Error {0}: {1}", name, infoLog);
		throw std::runtime_error(entry);
	}
}

void Shader::use()
{
	if(ACTIVE != this)
	{
		glUseProgram(prog_id);
		ACTIVE = this;
	}
}

void Shader::block_binding(const char* name, unsigned int binding)
{
	glUniformBlockBinding(prog_id, get_block_index(name), binding);
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

unsigned int Shader::get_block_index(const char* pos)
{
	auto it = uniform_block_indices.find(pos);

	if(it != uniform_block_indices.end())
	{
		return it->second;
	}

	return uniform_block_indices[pos] = glGetUniformBlockIndex(prog_id, pos);
}

unsigned int Shader::get(const char* pos)
{
	return operator[](pos);
}

