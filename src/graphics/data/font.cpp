
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

#include "mesh.hpp"
#include "arrays.hpp"
#include "font.hpp"

using namespace Sim::Graphics::Data;

struct Character
{
	unsigned long handle;
	float advance;
	glm::vec2 size;
	glm::vec2 bearing;
};

static Character chars[128];

void Font::init()
{
	FT_Library ft;
	FT_Face face;
	
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "Error: failed to init freetype\n";
		return;
	}

	if(FT_New_Face(ft, "../assets/font/DroidSans.ttf", 0, &face))
	{
		std::cout << "Error: failed to load freetype font\n";
		return;
	}

	int size = 256;
	float m = 1.0f / size;

	FT_Set_Pixel_Sizes(face, 0, size);

	GLuint texids[128];
	std::vector<glm::vec<4, unsigned char>> pixels;
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
		
		Character& c = chars[i];
		c.advance = face->glyph->advance.x * m / 64.0;
		c.size = {width * m, height * m};
		c.bearing = {offx * m, offy * m};

		if(c.size.x == 0 || c.size.y == 0)
		{
			c.handle = 0;
			continue;
		}

		pixels.resize(width * height);

		for(int i = 0; i < width * height; i++)
		{
			pixels[i] = glm::vec<4, unsigned char>(face->glyph->bitmap.buffer[i]);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &texids[i]);

		glTextureStorage2D(texids[i], 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texids[i], 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

		glTextureParameteri(texids[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texids[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texids[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texids[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		c.handle = glGetTextureHandleARB(texids[i]);
		glMakeTextureHandleResidentARB(c.handle);

		chars[i] = c;
	}

	FT_Done_FreeType(ft);
}

Mesh& Mesh::load_text(const char* text, double size)
{
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;

	float x = 0, y = size;
	unsigned int at = 0;

	if(text[0] == '\0')
	{
		this->vertices.clear();
		this->indices.clear();
		return *this;
	}

	for(unsigned int i = 0; text[i] != '\0'; i++)
	{
		char c = text[i];
		Character ch = chars[c];

		if(c == '\n')
		{
			x = 0;
			y += size;
			continue;
		}

		if(ch.handle == 0)
		{
			x += ch.advance * size;
			continue;
		}
		
		unsigned int index[6] = {
			at, at + 1, at + 3,
			at, at + 3, at + 2
		};

		float sx = x + ch.bearing.x * size;
		float sy = y - ch.bearing.y * size;
		float ex = sx + ch.size.x * size;
		float ey = sy + ch.size.y * size;

		vertices.push_back(Arrays::Vertex{.texid=ch.handle, .texpos={0, 0}, .pos={sx, sy, 0}, .normal={0, 0, -1}, .material={0, 0, 1}});
		vertices.push_back(Arrays::Vertex{.texid=ch.handle, .texpos={0, 1}, .pos={sx, ey, 0}, .normal={0, 0, -1}, .material={0, 0, 1}});
		vertices.push_back(Arrays::Vertex{.texid=ch.handle, .texpos={1, 0}, .pos={ex, sy, 0}, .normal={0, 0, -1}, .material={0, 0, 1}});
		vertices.push_back(Arrays::Vertex{.texid=ch.handle, .texpos={1, 1}, .pos={ex, ey, 0}, .normal={0, 0, -1}, .material={0, 0, 1}});
		indices.insert(indices.end(), &index[0], &index[6]);

		at += 4;
		x += ch.advance * size;
	}

	this->vertices = std::move(vertices);
	this->indices = std::move(indices);
	this->transforms.clear();

	return *this;
}

Mesh& Mesh::load_text(const char* text, double size, glm::vec2 align)
{
	glm::vec2 max;
	
	load_text(text, size);

	for(Arrays::Vertex& v : vertices)
	{
		if(v.pos.x > max.x)
		{
			max.x = v.pos.x;
		}

		if(v.pos.y > max.y)
		{
			max.y = v.pos.y;
		}
	}

	align *= max;

	for(Arrays::Vertex& v : vertices)
	{
		v.pos.x -= align.x;
		v.pos.y -= align.y;
	}

	return *this;
}

