#pragma once
#include "Config.h"

class AsBorder
{
public:
	AsBorder();
	void Init();
	void Draw(HDC hdc, RECT& paint_area);

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border);

	HPEN BG_Pen;
	HBRUSH BG_Brush;

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};
