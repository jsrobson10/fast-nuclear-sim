
#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) in sampler2D aTex;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec4 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out float brightness;
out flat sampler2D tex;
out vec2 texPos;

void main()
{
	vec4 pos = camera * model * aPos;
	vec3 cNormal = vec3(0.f, 0.f, 1.f) * mat3(camera * model);

	brightness = dot(normalize(aNormal), normalize(cNormal)) * 0.25f + 0.75f;

	gl_Position = projection * pos;
	texPos = aTexPos;
	tex = aTex;
}

