#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Font.h"

#include "Log.h"



Font::Font() : Module()
{
	name.Create("font");
}

Font::~Font()
{

}

// Load new texture from file path
int Font::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Fonts& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data
	app->tex->GetSize(font.texture, font.char_w, font.char_h);
	// totalLength ---	length of the lookup table
	font.totalLength = strlen(characters);
	// table ---------	All characters displayed in the same order as the texture
	strcpy_s(fonts[id].table, characters);
	// columns -------	Amount of chars per row of the texture
	font.columns = fonts[id].totalLength / rows;
	// char_w --------	Width of each character
	font.char_w /= fonts[id].totalLength / rows;
	// char_h --------	Height of each character
	font.char_h /= rows;

	++fontsCount;

	LOG("Successfully loaded BMP font from %s", texture_path);



	return id;
}

void Font::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		--fontsCount;

		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void Font::DrawText(int x, int y, int font_index, const char* text) const
{
	if (text == nullptr || font_index < 0 || font_index >= MAX_FONTS || fonts[font_index].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_index);
		return;
	}

	//LOG("----------------------%d----------------------", font_index);

	const Fonts* font = &fonts[font_index];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;
		// TODO 2: Find the character in the table, its position in the texture and then Blit
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}
