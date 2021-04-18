 #pragma once

#include "Module.h"

#include "SString.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Fonts
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;

};

class Font : public Module
{
public:

	// Constructor
	Font();

	// Destructor
	~Font();

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void DrawText(int x, int y, int fontIndex, const char* text) const;

	inline uint GetFontsCount() const { return fontsCount; };

private:
	// An array to keep track and store all loaded fonts
	Fonts fonts[MAX_FONTS];

	// The amount of fonts loaded into the array
	uint fontsCount = 0;
};