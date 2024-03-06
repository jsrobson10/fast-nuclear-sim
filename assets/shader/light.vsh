
#version 460 core

layout (location = 2) in vec3 aPos;
layout (location = 4) in vec4 aColour;
layout (location = 5) in vec3 aMaterial;
layout (location = 6) in float aTransformIndex;

layout (binding = 3) readonly buffer TransformBuffer
{
	mat4 transforms[];
};

uniform mat4 camera;

out flat int should_ignore;

mat4 load_model_mat(int index)
{
	return index < 0 ? mat4(1.f) : transforms[index];
}

void main()
{
	vec4 pos = vec4(aPos, 1.f);
	mat4 model = load_model_mat(int(aTransformIndex));
	mat4 mv = camera * model;

	gl_Position = mv * pos;
	should_ignore = int(aMaterial[2] > 0.f || aColour.a < 1.f);
}

