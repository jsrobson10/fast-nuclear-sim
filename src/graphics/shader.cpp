
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
layout (location = 2) in vec4 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;

out float brightness;
out flat sampler2D tex;
out vec2 texPos;

void main()
{
	vec4 pos = model * aPos;
	vec3 cNormal = vec3(0.f, 0.f, 1.f) * mat3(model);

	brightness = dot(normalize(aNormal), normalize(cNormal)) * 0.25f + 0.75f;

	gl_Position = projection * pos;
	texPos = aTexPos;
	tex = aTex;
}

)";

static const char* FRAGMENT_SHADER = R"(
#version 460 core
#extension GL_ARB_bindless_texture : require

in float brightness;
in flat sampler2D tex;
in vec2 texPos;

out vec4 FragColour;

uniform mat4 tex_mat;

void main()
{
	vec4 texdata = texture2D(tex, texPos);
	FragColour = tex_mat * texdata * vec4(vec3(brightness), 1);

	if(FragColour.a == 0) discard;
}

)";

static unsigned int prog_id;

int shader::gl_tex_mat;
int shader::gl_model;
int shader::gl_projection;

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
	
	gl_tex_mat = glGetUniformLocation(prog_id, "tex_mat");
	gl_model = glGetUniformLocation(prog_id, "model");
	gl_projection = glGetUniformLocation(prog_id, "projection");

	glUseProgram(prog_id);
	glDeleteShader(vsh_id);
	glDeleteShader(fsh_id);

	return prog_id;
}

