
#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in sampler2D aTex;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec4 aColour;
layout (location = 5) in vec3 aMaterial;
layout (location = 6) in float aTransformIndex;

uniform mat4 camera;
uniform mat4 projection;

layout (binding = 3) readonly buffer TransformBuffer1
{
	mat4 transforms[];
};

out VS_OUT {
	vec3 normal;
	vec4 colour;
	vec3 pos;
	vec2 tex_pos;
	vec3 material;
	float ambient;
} vout;

out flat sampler2D frag_tex;

mat4 load_model_mat(int index)
{
	return index < 0 ? mat4(1.f) : transforms[index];
}

float Map(float v, float i_min, float i_max, float o_min, float o_max)
{
	return o_min + (o_max - o_min) * (v - i_min) / (i_max - i_min);
}

void main()
{
	vec4 pos = vec4(aPos, 1.f);
	mat4 model = load_model_mat(int(aTransformIndex));
	mat4 mv = camera * model;
	mat4 mvp = projection * mv;

	vout.normal = normalize(mat3(model) * aNormal);
	vout.ambient = Map(dot(vout.normal, vec3(0.f, 0.f, 1.f)), -1.f, 1.f, 0.2f, 0.25f);
	vout.pos = (model * pos).xyz;
	vout.colour = aColour;
	vout.tex_pos = aTexPos;
	vout.material = aMaterial;
	frag_tex = aTex;
	
	gl_Position = mvp * pos;
}

