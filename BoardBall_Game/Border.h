#pragma once
//#include "Config.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGraphics_Object
{
public:
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area);
	void Draw_Floor(HDC hdc, RECT &paint_area);
	RECT Floor_Rect;
};
//------------------------------------------------------------------------------------------------------------