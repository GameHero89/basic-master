#include "Common.hpp"
#include "Interface.hpp"
#include "Natives.hpp"
#include "Enum.hpp"
#include "Keyboard.hpp"
#include "Fiber.hpp"


#define InterfaceSwitch() switch (CurrentSubmenu) 
#define submenu(menu) case menu: {
#define subend break; }

void Interface() {
	static bool init = true;
	if (init) {
		char Message[] = "Welcome to Basic!/Made by GameHero/Open with * or F10/Enjoy :D";
		PrintTextOnScreen(Message);
		init = false;
	}

	openKey = false;
	enterKey = false;
	backKey = false;
	leftKey = false;
	rightKey = false;
	upKey = false;
	downKey = false;

	static int timer = timeGetTime();
	if (timeGetTime() - timer > EnterDelay) {
		openKey = (IsKeyPressed(VK_F10) || IsKeyPressed(VK_MULTIPLY) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SCRIPT_RB) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RIGHT)));
		if (openKey) timer = timeGetTime();
	}

	if (openKey) {
		SetSubmenu(CurrentSubmenu ? nomenu : mainmenu);
	}

	if (CurrentSubmenu != nomenu) {
		//Disables Phone
		CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_PHONE, true);

		//Disables Controller relatet stuff
		CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
		int disabledControls[] = { 0, 19, 20, 74, 80, 85, 140, 141, 142, 337 };
		for (int i = 0; i < ARRAYSIZE(disabledControls); i++) {
			CONTROLS::DISABLE_CONTROL_ACTION(0, disabledControls[i], true);
		}

		static int timer = timeGetTime();
		static int timer1 = timeGetTime();
		static int timer2 = timeGetTime();

		if (timeGetTime() - timer > EnterDelay) {
			enterKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RDOWN) || IsKeyPressed(VK_NUMPAD5);
			backKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RRIGHT) || IsKeyPressed(VK_NUMPAD0);
			if (enterKey || backKey) timer = timeGetTime();
		}
		if (timeGetTime() - timer1 > HorizontalDelay) {
			leftKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_LEFT) || IsKeyPressed(VK_NUMPAD4);
			rightKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RIGHT) || IsKeyPressed(VK_NUMPAD6);
			if (leftKey || rightKey) timer1 = timeGetTime();
		}
		if (timeGetTime() - timer2 > VerticalDelay) {
			upKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_UP) || IsKeyPressed(VK_NUMPAD8);
			downKey = CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_DOWN) || IsKeyPressed(VK_NUMPAD2);
			if (upKey || downKey) timer2 = timeGetTime();
		}
	}

	if (backKey) {
		SetSubmenu(SubmenuStack.back());
	}

	if (upKey) {
		SelectedOption--;

	} if (downKey) {
		SelectedOption++;
	}

	if (SelectedOption < 1) {
		SelectedOption = StaticOptionCount;
	}
	if (SelectedOption > StaticOptionCount && StaticOptionCount) {
		SelectedOption = 1;
	}



	if (CurrentSubmenu != nomenu) {
		Header();
		InterfaceSwitch() {
			submenu(mainmenu)

			Button("Button", [] {
				PrintTextOnScreen((char*)"Button/Pressed");
				});
			static bool _bool = true;
			Break("Break");
			Toggle("Toggle", _bool);
			static int _int = 12;
			Number<int>("Int", _int, 0, 100);
			static float _float = 1.5f;
			Number<float>("Float", _float, 0, 10, 0.1f);

			Button("Unload", [] {
				g_Running = false;
				});
			Suboption("Settings", settings);

			subend

			submenu(settings)
			Number<float>("Pos X", PosX, 0.0f + (GetWidthScale(Width) / 2.f), 1.f - (GetWidthScale(Width) / 2.f), 0.01f);
			Number<float>("Pos Y", PosY, 0.0f, 1.f - (DrawY - PosY), 0.01f);
			Number<float>("Width", Width, 0.01f, 1.f, 0.01f);
			Break("Header");
			Toggle("Draw Header", DrawHeader);
			static int font = (int)HeaderFont;
			Number<int>("Header Font", font, 0, 7);
			if (font == 5) font = 7;
			if (font == 6) font = 4;
			HeaderFont = (Font)font;
			Number<float>("Header Height", HeaderHeight, 0.01f, 0.5f, 0.001f);
			Number<float>("Header Text Size", HeaderTextSize, 0.1f, 10.f, 0.01f);
			Break("Options");
			static int font2 = (int)OptionFont;
			Number<int>("Option Font", font2, 0, 7);
			if (font2 == 5) font2 = 7;
			if (font2 == 6) font2 = 4;
			OptionFont = (Font)font2;
			Number<float>("Option Height", OptionHeight, 0.01f, 0.5f, 0.0005f);
			Number<float>("Option Text Size", OptionTextSize, 0.1f, 10.f, 0.005f);
			Number<float>("Option Padding", OptionPadding, 0.0f, 0.5f, 0.001f);
			Break("Footer");
			Toggle("Draw Footer", DrawFooter);
			static int font3 = (int)FooterFont;
			Number<int>("Footer Font", font3, 0, 7);
			if (font3 == 5) font3 = 7;
			if (font3 == 6) font3 = 4;
			FooterFont = (Font)font3;
			Number<float>("Footer Height", FooterHeight, 0.01f, 0.5f, 0.001f);
			Number<float>("Footer Text Size", FooterTextSize, 0.1f, 10.f, 0.01f);
			subend
		}
		Footer();
	}

	if (!StaticOptionCount && OptionCount)
		StaticOptionCount = OptionCount;

	OptionCount = 0;
}



// BasicUI by GameHero

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

float GetWidthScale(float width) {
	return width * (1.77778 /*16:9*/ / GRAPHICS::_GET_ASPECT_RATIO(0));
}

bool IsVisible(int option) {
	if (SelectedOption <= MaxOptions) {
		if (option <= MaxOptions) return true;
	} else {
		if (option > (SelectedOption - MaxOptions) && option <= SelectedOption) return true;
	}
	return false;
}

bool IsSelected(int option) {
	return (option == SelectedOption);
}

void Header() {
	DrawY = PosY;
	if (DrawHeader) {
		Rect(PosX, DrawY + (HeaderHeight / 2.f), GetWidthScale(Width), HeaderHeight, HeaderRect);
		Text(Center, BASE_NAME, PosX, DrawY + (HeaderHeight / 2.f), HeaderTextSize, HeaderFont, HeaderText);
		DrawY += HeaderHeight;
	}
}

void Footer() {
	if (DrawFooter) {
		char text[20];
		memset(text, 0, sizeof(text));
		snprintf(text, sizeof(text), "%i / %i", SelectedOption, OptionCount); // c++ i hate you
		Text(Right, text, PosX + ((GetWidthScale(Width) / 2.f) - OptionPadding), DrawY + (FooterHeight / 2.f), FooterTextSize, FooterFont, FooterText);
		Text(Center, "BASIC", PosX, DrawY + (FooterHeight / 2.f), FooterTextSize, HeaderFont, FooterText);
		Rect(PosX, DrawY + (FooterHeight / 2.f), GetWidthScale(Width), FooterHeight, FooterRect);
		DrawY += FooterHeight;
	}
}

void Break(const char* text) {
	OptionCount++;
	if (IsVisible(OptionCount)) {
		Text(Center, text, PosX, DrawY + (OptionHeight / 2.f), OptionTextSize, Font::HouseScript, OptionText);
		Rect(PosX, DrawY + (OptionHeight / 2.f), GetWidthScale(Width), OptionHeight, BackgroundRect);
		IsSelected(OptionCount) ? Rect((PosX - (GetWidthScale(Width) / 2.f)) + (OptionPadding - 0.001f) / 2.f, DrawY + (OptionHeight / 2.f), GetWidthScale(Width) - (GetWidthScale(Width) - (OptionPadding - 0.001f)), OptionHeight, ScrollerRect) : void();
		DrawY += OptionHeight;
	}
}

void Button(const char* text, std::function<void()> function) {
	OptionCount++;
	if (IsVisible(OptionCount)) {
		Text(Left, text, PosX - ((GetWidthScale(Width) / 2.f) - OptionPadding), DrawY + (OptionHeight / 2.f), OptionTextSize, OptionFont, IsSelected(OptionCount) ? SelectedText : OptionText);
		Rect(PosX, DrawY + (OptionHeight / 2.f), GetWidthScale(Width), OptionHeight, BackgroundRect);
		IsSelected(OptionCount) ? Rect(PosX, DrawY + (OptionHeight / 2.f), GetWidthScale(Width), OptionHeight, ScrollerRect) : void();
		DrawY += OptionHeight;

		if (IsSelected(OptionCount) && enterKey) {
			function();
		}
	}
}

void Suboption(const char* text, Submenu submenu, std::function<void()> function) {
	Button(text);
	if (IsVisible(OptionCount)) {
		Text(Right, ">", PosX + ((GetWidthScale(Width) / 2.f) - OptionPadding), DrawY - (OptionHeight / 2.f), OptionTextSize * 1.15f, Font::Monospace, IsSelected(OptionCount) ? SelectedText : OptionText);

		if (IsSelected(OptionCount) && enterKey) {
			SetSubmenu(submenu);
			function();
		}
	}
}

void Toggle(const char* text, bool& _bool, std::function<void()> function) {
	Button(text);
	if (IsVisible(OptionCount)) {
		Text(Right, _bool ? "ON" : "OFF", PosX + ((GetWidthScale(Width) / 2.f) - OptionPadding), DrawY - (OptionHeight / 2.f), OptionTextSize, OptionFont, IsSelected(OptionCount) ? SelectedText : OptionText);

		if (IsSelected(OptionCount) && enterKey) {
			_bool ^= 1;
			function();
		}
	}
}

template <typename Variable>
void Number(const char* text, Variable& reference, Variable minimum, Variable maximum, Variable step) {
	Button(text);
	if (IsVisible(OptionCount)) {
		char righttext[20]; // pls everyone use sprintf and dont leak memory with strdup XD
		memset(righttext, 0, sizeof(righttext));
		snprintf(righttext, sizeof(righttext), "%g", round(reference * 10000) / 10000); // c++ i hate you
		Text(Right, righttext, PosX + ((GetWidthScale(Width) / 2.f) - OptionPadding), DrawY - (OptionHeight / 2.f), OptionTextSize, OptionFont, IsSelected(OptionCount) ? SelectedText : OptionText);

		if (IsSelected(OptionCount)) {
			if (leftKey) {
				reference > minimum ? reference -= step : reference = maximum;
			} if (rightKey) {
				reference < maximum ? reference += step : reference = minimum;
			}
		}
	}
}

void SetSubmenu(Submenu submenu) {
	bool inStack = false;
	for (int i = 0; i < SubmenuStack.size(); i++) {
		if (SubmenuStack.at(i) == submenu) {
			SubmenuStack.resize(i);
			inStack = true;
			break;
		}
	} if (!inStack) {
		if (CurOptionStack.size() < CurrentSubmenu + 1)
			CurOptionStack.resize(SelectedOption + 1);
		CurOptionStack.at(CurrentSubmenu) = SelectedOption;
		SubmenuStack.push_back(CurrentSubmenu);
		SelectedOption = 1;
	}
	else {
		SelectedOption = CurOptionStack.at(submenu);
	}
	CurrentSubmenu = submenu;
	StaticOptionCount = 0;
	OptionCount = 0;
}

void PrintTextOnScreen(char* Message) {
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
				Text(Center, Word, 0.5f, 0.5f, 2.1f, Font::HouseScript, { HeaderRect.r, HeaderRect.g, HeaderRect.b, (byte)alpha }, 0, 1);
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




