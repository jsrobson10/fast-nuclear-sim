
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/vec2.hpp>
#include <iostream>

#include "font.hpp"

using namespace sim::graphics;

font::character font::chars[128];

void font::init()
{
	FT_Library ft;
	FT_Face face;
	
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "Error: failed to init freetype\n";
		return;
	}

	if(FT_New_Face(ft, "/usr/share/fonts/noto/NotoSans-Regular.ttf", 0, &face))
	{
		std::cout << "Error: failed to load freetype font\n";
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 1024);

	GLuint texids[128];
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glCreateTextures(GL_TEXTURE_2D, 128, texids);

	for(int i = 0; i < 128; i++)
	{
		if(FT_Load_Char(face, (char)i, FT_LOAD_RENDER))
		{
			std::cout << "Error: failed to load glyph " << i << "\n";
		}
		
		character& c = chars[i];
		c.advance = face->glyph->advance.x;
		c.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
		c.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};

		if(c.size.x == 0 || c.size.y == 0)
		{
			c.handle = 0;
			continue;
		}

		glTextureStorage2D(texids[i], 1, GL_R8, c.size.x, c.size.y);
		glTextureSubImage2D(texids[i], 0, 0, 0, c.size.x, c.size.y, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTextureParameteri(texids[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texids[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texids[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texids[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		c.handle = glGetTextureHandleARB(texids[i]);
		glMakeTextureHandleResidentARB(c.handle);

		chars[i] = c;
	}
}

