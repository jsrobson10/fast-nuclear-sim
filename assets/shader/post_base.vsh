
#version 460 core

const vec2 QuadVertices[6] = vec2[6](
	vec2(-1.0,  1.0),
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0),
	vec2(-1.0,  1.0),
	vec2( 1.0, -1.0),
	vec2( 1.0,  1.0)
);

out vec2 FragPos;

void main()
{
	vec2 vertex = QuadVertices[gl_VertexID];
	FragPos = vertex * 0.5f + 0.5f;
	gl_Position = vec4(vertex, 0.f, 1.f);
}

