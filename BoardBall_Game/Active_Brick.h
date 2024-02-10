#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AActive_Brick
{
public:
	AActive_Brick();
	void Act(HWND hwnd);
	void Draw(HDC hdc);

private:
	RECT Brick_Rect;
	HPEN Brick_Pen;
	HBRUSH Brick_Brush;
	int Fade_Step;
	static const int Max_Fade_Step = 80;
};
//------------------------------------------------------------------------------------------------------------
