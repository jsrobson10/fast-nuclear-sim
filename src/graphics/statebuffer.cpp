
#include "statebuffer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace Sim::Graphics;

static GLuint ssbo;

void StateBuffer::init()
{
	glGenBuffers(1, &ssbo);
	set({glm::mat4(1), glm::mat4(1)});
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo);
}

void StateBuffer::set(const StateBuffer& sb)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(sb), &sb, GL_STREAM_DRAW);
}

