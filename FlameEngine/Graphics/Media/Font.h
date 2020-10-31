#pragma once

#include "../../dll/nchfx.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../Graphics/Texture.h"

EXPORT_STRUCT Character 
{
	Texture* texture;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};


EXPORT_CLASS Font
{
public:
	Font(const char* _fontFamily, int size);

	std::map<GLchar, Character> Characters;
};
