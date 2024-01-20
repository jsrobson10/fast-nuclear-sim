
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"
#include "window.hpp"

using namespace sim::graphics;

static const char* VERTEX_SHADER = R"(
#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in sampler2D aTex;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aPos;

out flat sampler2D tex;
out vec2 texPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	texPos = aTexPos;
	tex = aTex;
}

)";

static const char* FRAGMENT_SHADER = R"(
#version 460 core
#extension GL_ARB_bindless_texture : require

in flat sampler2D tex;
in vec2 texPos;

out vec4 FragColour;

void main()
{
	FragColour = vec4(1) * texture2D(tex, texPos);
}

)";

static unsigned int prog_id;

static int load_shader(const char** src, int type)
{
	int id = glCreateShader(type);

	glShaderSource(id, 1, src, nullptr);
	glCompileShader(id);

	return id;
}

unsigned int shader::init_program()
{
	int success;
	int vsh_id = load_shader(&VERTEX_SHADER, GL_VERTEX_SHADER);
	int fsh_id = load_shader(&FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
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

	glUseProgram(prog_id);
	glDeleteShader(vsh_id);
	glDeleteShader(fsh_id);

	return prog_id;
}

