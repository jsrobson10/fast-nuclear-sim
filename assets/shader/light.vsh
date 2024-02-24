
#version 460 core

layout (location = 2) in vec4 aPos;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * aPos;
}

