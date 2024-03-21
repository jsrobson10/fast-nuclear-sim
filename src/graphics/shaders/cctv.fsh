R"GLSL(

uniform sampler2D tex;

in VS_OUT
{
	vec2 tex_pos;
} vin;

out vec4 frag_colour;

void main()
{
	frag_colour = texture(tex, vin.tex_pos);
}

)GLSL";
