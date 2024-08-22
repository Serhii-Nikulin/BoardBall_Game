#include "Config.h"

const double AsConfig::D_Global_Scale = 3.0;
const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Color(237, 38, 36);
const AColor AsConfig::Blue_Color(63, 72, 204);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);

const AColor AsConfig::Red_Higlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);

const AColor AsConfig::Teleport_Color(AsConfig::Blue_Color, AsConfig::Global_Scale, AsConfig::BG_Color);

const AColor AsConfig::Advert_Blue_Table_Color(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Advert_Red_Table_Color(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);

const double AsConfig::Acceleration = 1.001;
const double AsConfig::Ball_Normal_Speed = 3.0;
//------------------------------------------------------------------------------------------------------------
AColor::AColor():
	R(0), G(0), B(0), Pen(0), Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	Brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor& color, int pen_size)
	: R(color.R), G(color.G), B(color.B)
{
	Pen = CreatePen(PS_SOLID, pen_size, color.Get_RGB() );
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &pen_color, int pen_size, const AColor &brush_color)
	: R(0), G(0), B(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, pen_color.Get_RGB() );
	Brush = CreateSolidBrush(brush_color.Get_RGB() );
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
void AColor::Select_Pen(HDC hdc) const
{
	SelectObject(hdc, Pen);
}
//------------------------------------------------------------------------------------------------------------
HBRUSH AColor::Get_Brush() const
{
	return Brush;
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
HWND AsConfig::Hwnd;

double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = false;
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
void AsConfig::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
	int radius = corner_radius * Global_Scale;
	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------