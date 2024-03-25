
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/vec2.hpp>
#include <iostream>
#include <vector>
#include <format>

#include "../shader.hpp"
#include "mesh.hpp"
#include "arrays.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "font.hpp"

using namespace Sim::Graphics::Data;

Font Fonts::BASE;
Font Fonts::MONO;

void Fonts::init()
{
	BASE.init("DroidSans", 64);
	MONO.init("DroidSansMono", 64);
}

void Font::init(const std::string& name, int size)
{
	FT_Library ft;
	FT_Face face;
	
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "Error: failed to init freetype\n";
		return;
	}

	if(FT_New_Face(ft, std::format("../assets/font/{}.ttf", name).c_str(), 0, &face))
	{
		std::cout << "Error: failed to load freetype font\n";
		return;
	}

	texel_size = 1.0f / size;
	FT_Set_Pixel_Sizes(face, 0, size);

	for(int i = 0; i < 128; i++)
	{
		if(FT_Load_Char(face, (char)i, FT_LOAD_RENDER))
		{
			std::cout << "Error: failed to load glyph " << i << "\n";
		}

		int width = face->glyph->bitmap.width;
		int height = face->glyph->bitmap.rows;
		int offx = face->glyph->bitmap_left;
		int offy = face->glyph->bitmap_top;
		
		Character& c = characters[i];
		c.advance = face->glyph->advance.x * texel_size / 64.0;
		c.size = {width * texel_size, height * texel_size};
		c.bearing = {offx * texel_size, offy * texel_size};

		if(c.size.x == 0 || c.size.y == 0)
		{
			continue;
		}

		std::vector<uint8_t> buffer(width * height);

		for(int i = 0; i < width * height; i++)
		{
			buffer[i] = face->glyph->bitmap.buffer[i];
		}

		int swizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
		c.handle = Texture::load_mem(buffer.data(), width, height, 1, swizzleMask, GL_CLAMP_TO_EDGE);
	}

	FT_Done_FreeType(ft);
}

Font::Font()
{
}

float Font::calc_line_width(const char* text, float size) const
{
	float x = 0;

	for(int i = 0; text[i] != '\0' && text[i] != '\n'; i++)
	{
		char c = text[i];
		x += characters[c].advance * size;
	}

	return x;
}

float Font::calc_height(const char* text, float size) const
{
	float y = 0;
	float off = 0;
	bool at_top = true;

	for(int i = 0; text[i] != '\0'; i++)
	{
		char c = text[i];

		if(c == '\n')
		{
			y += size;
			at_top = false;
		}

		else if(at_top)
		{
			off = std::max(off, characters[c].bearing.y * size);
		}
	}

	return y + size * 2 - off;
}

void Font::load_text(Mesh& rmesh, const std::string& text, load_text_t t) const
{
	if(text[0] == '\0')
	{
		return;
	}

	float x = 0;
	float y = t.size;
	float off_x = 0;
	float off_y = 0;
	unsigned int at = 0;

	if(t.align.x > 0)
	{
		off_x = calc_line_width(text.c_str(), t.size) * t.align.x;
	}

	if(t.align.y > 0)
	{
		off_y = calc_height(text.c_str(), t.size) * t.align.y;
	}

	for(int i = 0; i < text.size(); i++)
	{
		char c = text[i];
		const Character& ch = characters[c];

		if(c == '\n')
		{
			x = 0;
			y += t.size;
			off_x = calc_line_width(text.c_str() + i + 1, t.size) * t.align.x;
			continue;
		}

		if(ch.handle == 0)
		{
			x += ch.advance * t.size;
			continue;
		}

		float sx = x + ch.bearing.x * t.size - off_x;
		float sy = y - ch.bearing.y * t.size - off_y;
		float ex = sx + ch.size.x * t.size;
		float ey = sy + ch.size.y * t.size;

		Mesh::Primitive<4, 6> prim {
			.vertex_base = t.prim_base,
			.indices = {0, 1, 3, 0, 3, 2},
			.vertices = {
				{.texpos={0, 0}, .pos={sx, sy, 0}},
				{.texpos={0, 1}, .pos={sx, ey, 0}},
				{.texpos={1, 0}, .pos={ex, sy, 0}},
				{.texpos={1, 1}, .pos={ex, ey, 0}},
			},
		};

		prim.vertex_base.tex_diffuse = ch.handle;
		rmesh.add(prim, t.mat, t.bake);
		x += ch.advance * t.size;
	}
}

