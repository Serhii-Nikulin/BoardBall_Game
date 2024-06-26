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

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border);
};
//------------------------------------------------------------------------------------------------------------