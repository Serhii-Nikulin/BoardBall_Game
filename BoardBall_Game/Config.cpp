#include "Config.h"

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Brick_Color(237, 38, 36);
const AColor AsConfig::Blue_Brick_Color(63, 72, 204);
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b)
{}
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;
HWND AsConfig::Hwnd;
double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = true;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
	Create_Pen_Brush(AsConfig::BG_Color, AsConfig::BG_Pen, AsConfig::BG_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor& color, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
	return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------