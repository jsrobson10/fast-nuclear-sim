
#version 460 core

layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec4 aPos;

out vec2 texPos;

void main()
{
	texPos = aTexPos;
	gl_Position = aPos;
}

