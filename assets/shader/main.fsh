
#version 460 core
#extension GL_ARB_bindless_texture : require

in float brightness;
in flat sampler2D tex;
in vec2 texPos;

out vec4 FragColour;

uniform mat4 tex_mat;

void main()
{
	vec4 texdata = texture2D(tex, texPos);
	FragColour = tex_mat * texdata * vec4(vec3(brightness), 1);

	if(FragColour.a == 0) discard;
}

