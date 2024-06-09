#include "Config.h"

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Brick_Color(237, 38, 36);
const AColor AsConfig::Blue_Brick_Color(63, 72, 204);
const AColor AsConfig::White_Color(255, 255, 255);
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b)
{
	Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	Brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
	return RGB(R, G, B);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush); 
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;
HWND AsConfig::Hwnd;

HPEN AsConfig::Letter_Pen;
HPEN AsConfig::Brick_Red_Pen, AsConfig::Brick_Blue_Pen, AsConfig::Brick_White_Pen;
HBRUSH AsConfig::Brick_Red_Brush, AsConfig::Brick_Blue_Brush, AsConfig::Brick_White_Brush;

double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = false;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
	AsConfig::Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, AsConfig::Brick_Blue_Pen, AsConfig::Brick_Blue_Brush);
	AsConfig::Create_Pen_Brush(AsConfig::Red_Brick_Color, AsConfig::Brick_Red_Pen, AsConfig::Brick_Red_Brush);
	AsConfig::Create_Pen_Brush(AsConfig::White_Color, AsConfig::Brick_White_Pen, AsConfig::Brick_White_Brush);

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