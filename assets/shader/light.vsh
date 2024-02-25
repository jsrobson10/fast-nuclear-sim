
#version 460 core

layout (location = 2) in vec4 aPos;
layout (location = 5) in vec3 aMaterial;

uniform mat4 model;
uniform mat4 camera;

out float emissive;

void main()
{
	gl_Position = camera * model * aPos;
	emissive = aMaterial[2];
}

