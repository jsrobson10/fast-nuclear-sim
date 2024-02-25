
#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadow_mats[6];

in float emissive[];
out vec3 frag_pos;

void main()
{
	if(emissive[0] > 0) return;

	for(int i = 0; i < 6; i++)
	{
		gl_Layer = i;

		for(int j = 0; j < 3; j++)
		{
			vec4 fp = gl_in[j].gl_Position;
			gl_Position = shadow_mats[i] * fp;
			frag_pos = fp.xyz;

			EmitVertex();
		}

		EndPrimitive();
	}
}

