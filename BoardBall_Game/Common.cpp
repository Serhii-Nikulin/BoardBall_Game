#include "Common.h"

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




//AColor
AColor::~AColor()
{
	DeleteObject(Pen);
	DeleteObject(Brush);
}
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




//AGame_Objects_Set
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Begin_Movement()
{
	int index = 0;
	AGame_Object *object;

	while(Get_Next_Game_Object(index, &object) )
		object->Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Finish_Movement()
{
	int index = 0;
	AGame_Object *object;

	while(Get_Next_Game_Object(index, &object) )
		object->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Shift_Per_Step(double max_speed)
{
	int index = 0;
	AGame_Object *object;

	while(Get_Next_Game_Object(index, &object) )
		object->Shift_Per_Step(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AGame_Objects_Set::Get_Speed()
{
	int index = 0;
	AGame_Object *object;
	double current_speed, max_speed = 0;

	while(Get_Next_Game_Object(index, &object) )
	{
		current_speed = object->Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Act()
{
	int index = 0;
	AGame_Object *object;

	while (Get_Next_Game_Object(index, &object) )
		object->Act();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Draw(HDC hdc, RECT &paint_area)
{
	int index = 0;
	AGame_Object *object;
	
	while(Get_Next_Game_Object(index, &object) )
		object->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	int index = 0;
	AGame_Object *object;

	while(Get_Next_Game_Object(index, &object) )
		object->Clear_Prev_Animation(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AGame_Objects_Set::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------