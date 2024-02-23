
#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in sampler2D aTex;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec4 aPos;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec4 aColour;
layout (location = 5) in vec3 aMaterial;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out VS_OUT {
	vec3 normal;
	vec4 colour;
	vec3 pos;
	vec2 tex_pos;
	vec3 material;
} vout;

out flat sampler2D frag_tex;

void main()
{
	mat4 mvp = camera * model;
	vec4 pos = mvp * aPos;

	vout.normal = mat3(model) * aNormal;
	vout.pos = (model * aPos).xyz;
	vout.colour = aColour;
	vout.tex_pos = aTexPos;
	vout.material = aMaterial;
	frag_tex = aTex;
	
	gl_Position = projection * pos;
}

