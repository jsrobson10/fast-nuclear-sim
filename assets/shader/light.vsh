
#version 460 core

#define MAX_LIGHTS 6

layout (location = 1) in vec3 aPos;
layout (location = 2) in vec4 aColour;
layout (location = 6) in int aTransformIndex;

layout (binding = 3) readonly buffer TransformBuffer
{
	mat4 transforms[];
};

out flat int should_ignore;

uniform vec3 light_pos;
uniform int light_pass;

mat4 load_model_mat(int index)
{
	return index < 0 ? mat4(1.f) : transforms[index];
}

void main()
{
	vec4 pos = vec4(aPos, 1.f);
	mat4 model = load_model_mat(aTransformIndex);

	gl_Position = model * pos - vec4(light_pos, 0.f);
	should_ignore = int(aColour.a < 1.f);
}

