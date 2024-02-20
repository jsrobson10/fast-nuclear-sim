
#version 460 core

uniform int samples;
uniform sampler2D tex;

in vec2 texPos;

out vec4 FragColour;

void main()
{
	vec2 texel_size = 1.f / vec2(textureSize(tex, 0));
	float samples_n = pow(samples * 2 + 1, 2);
	vec4 colour;
	
	for(int x = -samples; x <= samples; x++)
	for(int y = -samples; y <= samples; y++)
	{
		vec2 off = texel_size * vec2(x, y);
		colour += texture2D(tex, texPos + off);
	}

	FragColour = vec4((colour / samples_n).rgb, 1);
}

