
#version 460 core

in vec3 frag_pos;

uniform float far_plane;

void main()
{
	float distance = length(frag_pos);
	gl_FragDepth = distance / far_plane + 1e-4f;
}

