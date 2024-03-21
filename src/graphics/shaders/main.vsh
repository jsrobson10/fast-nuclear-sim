R"GLSL(

#ifdef USE_BINDLESS_TEXTURES
#extension GL_ARB_bindless_texture : require
#endif

layout (location = 0) in vec2 aTexPos;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec4 aColour;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aNormal;
layout (location = 6) in vec3 aMaterial;
layout (location = 7) in int aTransformIndex;

#ifdef USE_BINDLESS_TEXTURES

layout (location = 8) in sampler2D aTexDiffuse;
layout (location = 9) in sampler2D aTexNormal;

out flat sampler2D frag_tex_diffuse;
out flat sampler2D frag_tex_normal;

#else

layout (location = 8) in uint aTexDiffuse;
layout (location = 9) in uint aTexNormal;

out flat uint frag_tex_diffuse;
out flat uint frag_tex_normal;

#endif

uniform mat4 camera;
uniform mat4 projection;

layout (std430, binding = 3) readonly buffer TransformBuffer
{
	mat4 transforms[];
};

out VS_OUT {
	mat3 tbn;
	vec3 pos;
	vec2 tex_pos;
	flat vec4 colour;
	flat vec3 material;
} vout;

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
	mat4 model = load_model_mat(aTransformIndex);
	mat4 mv = camera * model;
	mat4 mvp = projection * mv;

	vout.tbn = mat3(model) * mat3(aTangent, aBitangent, aNormal);
	vout.pos = (model * pos).xyz;
	vout.tex_pos = aTexPos;
	vout.colour = aColour;
	vout.material = aMaterial.xyz;
	frag_tex_diffuse = aTexDiffuse;
	frag_tex_normal = aTexNormal;
	
	gl_Position = mvp * pos;
}

)GLSL";
