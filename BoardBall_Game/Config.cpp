#include "Config.h"


const AColor AsConfig::Red_Brick_Color(237, 38, 36);
const AColor AsConfig::Blue_Brick_Color(63, 72, 204);
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b)
{}
//------------------------------------------------------------------------------------------------------------




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