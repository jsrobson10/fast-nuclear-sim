
#version 460 core

layout (location = 2) in vec4 aPos;
layout (location = 4) in vec4 aColour;
layout (location = 5) in vec3 aMaterial;

uniform mat4 model;
uniform mat4 camera;

out float emissive;
out float base_transparency;

void main()
{
	gl_Position = camera * model * aPos;
	base_transparency = 1.f - aColour.a;
	emissive = aMaterial[2];
}

