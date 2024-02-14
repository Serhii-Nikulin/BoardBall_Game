#pragma once
#include "Config.h"

enum EPlatform_State{
EPS_Missing, EPS_Normal, EPS_Meltdown, EPS_Roll_In
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
	void Set_State(EPlatform_State platform_state);

	int Width;
	int X_Pos;
	int X_Step;

private:
	void Draw_Circle_Highlight(HDC hdc);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	void Draw_Roll_In_State(HDC hdc, RECT &paint_area);

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
	static const int Meltdown_Speed = 4;

	static const int Normal_Width = 28 * AsConfig::Global_Scale;
	static int Meltdown_Platform_Y_Pos[Normal_Width];
};
//------------------------------------------------------------------------------------------------------------