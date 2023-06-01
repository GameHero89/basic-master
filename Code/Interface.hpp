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

enum Submenu : short {
	nomenu,
	mainmenu,
	settings,
};

enum TextAllinement : byte {
	Right,
	Left,
	Center
};

float GetWidthScale(float width);
void Rect(float x, float y, float width, float height, Color color);
void Sprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation);
void Text(TextAllinement allinement, const char* text, float x, float y, float size, Font font, Color color, bool outline = false, bool shadow = false);
void PrintTextOnScreen(char* Message);

void Header();
void Break(const char* text);
void Button(const char* text, std::function<void()> function = [] {});
void Suboption(const char* text, Submenu submenu, std::function<void()> function = [] {});
void Toggle(const char* text, bool& _bool, std::function<void()> function = [] {});
template <typename Variable>
void Number(const char* text, Variable& reference, Variable minimum, Variable maximum, Variable step = 1);
void Footer();

void SetSubmenu(Submenu submenu);

void Interface();

//Positioning
inline float PosX = 0.82f;
inline float PosY = 0.1f;
inline float DrawY = PosY;
inline float Width = 0.24f;

// Header
inline bool DrawHeader = true;
inline float HeaderHeight = 0.11f;
inline float HeaderTextSize = 1.3f;
inline Font HeaderFont = Font::Pricedown;
inline Color HeaderText{ 255, 255, 255, 255 };
inline Color HeaderRect{ 115, 122, 250, 255 };

//Footer
inline bool DrawFooter = true;
inline float FooterHeight = 0.025f;
inline float FooterTextSize = 0.5f;
inline Font FooterFont = Font::Pricedown;
inline Color FooterText{ 255, 255, 255, 255 };
inline Color FooterRect{ 115, 122, 250, 255 };

// Options
inline float OptionHeight = 0.04f;
inline float OptionTextSize = 0.5f;
inline Font OptionFont = Font::ChaletLondon;
inline float OptionPadding = 0.005f;
inline Color OptionText{ 255, 255, 255, 255 };
inline Color SelectedText{ 2, 2, 2, 255 };
inline Color ScrollerRect{ 250, 250, 250, 250 };
inline Color BackgroundRect{ 0, 0, 0, 220 };

// Input
inline std::uint32_t HorizontalDelay = 90;
inline std::uint32_t VerticalDelay = 100;
inline std::uint32_t EnterDelay = 120;

inline bool openKey = false;
inline bool enterKey = false;
inline bool backKey = false;
inline bool leftKey = false;
inline bool rightKey = false;
inline bool upKey = false;
inline bool downKey = false;

inline size_t MaxOptions = 10;
inline size_t OptionCount = 0;
inline size_t StaticOptionCount = 0;
inline size_t SelectedOption = 0;
inline std::vector<size_t> CurOptionStack;
inline Submenu CurrentSubmenu = nomenu;
inline std::vector<Submenu> SubmenuStack;



