
#version 460 core

in vec4 frag_pos;
out vec4 frag_colour;

void main()
{
	float distance = length(frag_pos);
	frag_colour = vec4(vec3(distance / 50.0f), 1);
	gl_FragDepth = distance / 50.0f;
}

