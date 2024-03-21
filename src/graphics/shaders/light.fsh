R"GLSL(

in vec3 frag_pos;

uniform float far_plane;

void main()
{
	float distance = length(frag_pos);
	gl_FragDepth = distance / far_plane;
}

)GLSL";
