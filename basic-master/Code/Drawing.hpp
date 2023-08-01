#pragma once
#include "Types.hpp"
#include "Enum.hpp"

enum class Font : byte
{
	ChaletLondon = 0,
	HouseScript = 1,
	Monospace = 2,
	Wingdings = 3,
	ChaletComprimeCologne = 4,
	Pricedown = 7
};

enum TextAllinement : byte {
	Right,
	Left,
	Center
};

void Rect(float x, float y, float width, float height, Color color);
void Sprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation);
void Text(TextAllinement allinement, const char* text, float x, float y, float size, Font font, Color color, bool outline = false, bool shadow = false);
void PrintMessageOnScreen(char* Message, Font font, Color color);