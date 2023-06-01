#include "Common.hpp"
#include "Drawing.hpp"
#include "Natives.hpp"
#include "Enum.hpp"
#include "Keyboard.hpp"
#include "Fiber.hpp"

void Rect(float x, float y, float width, float height, Color color) {
	GRAPHICS::DRAW_RECT(x, y, width, height, color.r, color.g, color.b, color.a);
}

void Sprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation) {
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict)) {
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, false);
	}
	if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict)) {
		GRAPHICS::DRAW_SPRITE(dict, texture, x, y, width, height, rotation, color.r, color.g, color.b, color.a);
	}
}

void Text(TextAllinement allinement, const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow) {
	// Added Centering for text 
	// Unbelievable that gta cant
	// center their texts right
	// and use uniform sizes
	// sadly bcs char sprites 
	// differ in size the fonts
	// arent perfectly the same size

	float scale = 1.f;
	switch (font) {
	case Font::ChaletLondon:
		scale = 0.75f;
		break;
	case Font::HouseScript:
		scale = 0.93f;
		break;
	case Font::Monospace:
		scale = 0.85f;
		break;
	case Font::Wingdings:
		scale = 0.65f;
		break;
	case Font::ChaletComprimeCologne:
		scale = 0.85f;
		break;
	case Font::Pricedown:
		scale = 1.05f;
		size = (size * 0.8f); // fix for pricedown being very oversize
		break;
	}

	if (allinement == Right) {
		UI::SET_TEXT_WRAP(0.f, x);
		UI::SET_TEXT_RIGHT_JUSTIFY(true);
	}
	if (allinement == Center) {
		UI::SET_TEXT_CENTRE(true);
	}
	UI::SET_TEXT_SCALE(size * scale, size * scale);
	UI::SET_TEXT_FONT((int)(font));
	UI::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
	if (outline)
		UI::SET_TEXT_OUTLINE();
	if (shadow)
		UI::SET_TEXT_DROP_SHADOW();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	// 18.01801801801802 is the text to rect size converter apparrently, its used by GetTextScale
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y - (((size * scale) / (scale * 18.01801801801802)) / 2));
}

void PrintMessageOnScreen(char* Message, Font font, Color color) {
	//divide lines by slash "/"

	int blurrspeed = 380;
	GRAPHICS::_TRANSITION_TO_BLURRED(blurrspeed);
	char Word[100];
	memset(Word, 0, sizeof(Word));
	int count = 0;
	int wordcount = 0;
	while (g_Running) {
		char _char = Message[count];
		if (_char != '/' && _char != '\0') {
			Word[wordcount] = _char;
			wordcount++;
			count++;
		}
		else {
			int timer = timeGetTime();
			int delay = 900;
			int alpha = 0;
			while ((timeGetTime() - timer < delay) && g_Running) {
				if (alpha < 255) {
					alpha += (255 / ((1.f / GAMEPLAY::GET_FRAME_TIME()) / (1000 / (delay / 4.f))));
				}
				if (alpha > 255) alpha = 255;
				Text(Center, Word, 0.5f, 0.5f, 2.1f, font, { color.r, color.g, color.b, (byte)alpha }, 0, 1);
				g_Fiber->Wait();
			}
			memset(Word, 0, sizeof(Word));
			wordcount = 0;
			count++;

			if (_char == '\0') {
				GRAPHICS::_TRANSITION_FROM_BLURRED(blurrspeed);
				break;
			}
		}
		g_Fiber->Wait();
	}
}

