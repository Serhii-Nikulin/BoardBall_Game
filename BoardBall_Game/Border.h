#pragma once
#include "Config.h"

class AsBorder
{
public:
	AsBorder();
	void Init();
	void Draw(HDC hdc, RECT& paint_area);

	static const int Border_X_Offset = 6;//Min_X
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;
private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border);

	HPEN BG_Pen;
	HBRUSH BG_Brush;

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};
//------------------------------------------------------------------------------------------------------------