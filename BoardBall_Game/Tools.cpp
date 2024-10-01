#include "Tools.h"

//------------------------------------------------------------------------------------------------------------
//AGraphics_Object
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
//AMover
AMover::~AMover()
{
}
//------------------------------------------------------------------------------------------------------------




//AHit_Checker
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double next_pos, double eval_dist, double radius, double min_value, double max_value)
{
	if (eval_dist > radius)
		return false;

	//value * value + eval_pos * eval_pos = radius * radius

	double value = sqrt(radius * radius - eval_dist * eval_dist);

	if ((next_pos + value) > min_value and (next_pos - value) < max_value)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(double x_pos, double y_pos)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------




//AColor
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