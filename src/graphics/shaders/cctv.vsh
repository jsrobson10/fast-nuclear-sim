R"GLSL(

layout (location = 0) in vec2 aTexPos;
layout (location = 1) in vec3 aPos;

layout (binding = 6) readonly buffer StateBuffer
{
	mat4 camera;
	mat4 projection;
};

out VS_OUT
{
	vec2 tex_pos;
} vout;

void main()
{
	vout.tex_pos = aTexPos;
	
	gl_Position = projection * camera * vec4(aPos, 1);
}

)GLSL";
