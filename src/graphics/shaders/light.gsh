R"GLSL(

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadow_mats[6];
uniform int light_pass;

in flat int should_ignore[];
out vec3 frag_pos;

void main()
{
	if(should_ignore[0] != 0) return;

	int pass_offset = light_pass * 6;

	for(int i = 0; i < 6; i++)
	{
		gl_Layer = i + pass_offset;

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

)GLSL";
