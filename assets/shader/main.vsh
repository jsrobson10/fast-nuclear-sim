
#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in sampler2D aTex;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec4 aPos;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec4 aColour;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec4 colour;
out flat sampler2D tex;
out vec2 texPos;

void main()
{
	vec4 pos = camera * model * aPos;
	vec3 cNormal = vec3(0.f, 0.f, 1.f) * mat3(camera * model);

//	float brightness = dot(normalize(aNormal), normalize(cNormal)) * 0.25f + 0.75f;

	colour = aColour;// * vec4(vec3(brightness), 1);
	gl_Position = projection * pos;
	texPos = aTexPos;
	tex = aTex;
}

