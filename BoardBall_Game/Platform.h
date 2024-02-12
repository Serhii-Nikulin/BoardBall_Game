#pragma once
#include "Config.h"

enum EPlatform_State{
EPS_Missing, EPS_Normal, EPS_Meltdown
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Redraw(HWND hwnd);
	void Act(HWND Hwnd);
	int Width;
	int X_Pos;
	int X_Step;

private:
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);

	EPlatform_State Platform_State;
	HBRUSH Platform_Inner_Brush;
	HPEN Platform_Inner_Pen;
	HPEN Platform_Circle_Pen;
	HBRUSH Platform_Circle_Brush;
	HPEN Highlight_Pen;
	RECT Prev_Platform_Rect, Platform_Rect;
	int Inner_Width;
	static const int Inner_Height = 5;
	static const int Circle_Size = 7;
	static const int Height = 7;
	int Meltdown_Y_Pos;
};
//------------------------------------------------------------------------------------------------------------