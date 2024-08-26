#pragma once
//#include "Config.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker
{
public:
	AsBorder();
	void Init();
	void Draw(HDC hdc, RECT& paint_area);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	void Redraw_Floor();

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area);
	void Draw_Floor(HDC hdc, RECT &paint_area);
	RECT Floor_Rect;
};
//------------------------------------------------------------------------------------------------------------