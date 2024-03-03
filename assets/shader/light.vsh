
#version 460 core

layout (location = 2) in vec4 aPos;
layout (location = 4) in vec4 aColour;
layout (location = 5) in vec3 aMaterial;
layout (location = 6) in float aTransformIndex;

uniform mat4 model;
uniform mat4 camera;

out flat int should_ignore;

void main()
{
	gl_Position = camera * model * aPos;
	should_ignore = int(aTransformIndex >= 0.f || aMaterial[2] > 0.f || aColour.a < 1.f);
}

