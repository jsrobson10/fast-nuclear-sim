
#version 460 core

uniform sampler2D tex;
uniform vec2 direction;
uniform int samples;

in vec2 FragPos;
out vec4 FragColour;

void main()
{
	int radius = (samples - 1) / 2;
	ivec2 size = textureSize(tex, 0);
	vec2 step = direction / size;
	vec4 sum = vec4(0.f);

	for(int i = -radius; i <= radius; i++)
	{
		vec2 offset = vec2(i) * step;
		sum += texture(tex, FragPos + offset);
	}

	FragColour = sum / float(samples);
}

