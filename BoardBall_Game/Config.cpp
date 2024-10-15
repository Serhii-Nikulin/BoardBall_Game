#include "Config.h"

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Color(237, 38, 36);
const AColor AsConfig::Blue_Color(63, 72, 204);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);

const AColor AsConfig::Red_Higlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);

const AColor AsConfig::Teleport_Color(AsConfig::Blue_Color, AsConfig::Global_Scale, AsConfig::BG_Color);

const AColor AsConfig::Advert_Blue_Table_Color(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Advert_Red_Table_Color(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);

const AColor AsConfig::Monster_Dark_Red_Color(191, 31, 31);
const AColor AsConfig::Monster_Cornea_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3, AsConfig::White_Color);
const AColor AsConfig::Monster_Iris_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3, AsConfig::Blue_Color);
const AColor AsConfig::BG_Outline_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3);

const double AsConfig::Acceleration = 1.001;
const double AsConfig::Ball_Normal_Speed = 3.0 * 2;
//------------------------------------------------------------------------------------------------------------
HWND AsConfig::Hwnd;

double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = true;
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
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------




//AsTools
//------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{
	return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;
	color.Select(hdc);

	Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
	int radius = corner_radius * AsConfig::Global_Scale;
	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------