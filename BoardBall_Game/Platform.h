#pragma once
#include "Falling_Letter.h"

enum EPlatform_State{
EPS_Missing, EPS_Ready, EPS_Normal, EPS_Meltdown, EPS_Roll_In, EPS_Expand_Roll_In
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker
{
public:
	AsPlatform();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	void Draw(HDC hdc, RECT &paint_area);
	void Redraw();
	void Act();
	void Set_State(EPlatform_State platform_state);
	EPlatform_State Get_State();
	void Move(bool to_left);
	bool Hit_By(AFalling_Letter *falling_letter);

	int Width;
	int X_Pos;
	int X_Step;
	int Inner_Width;

private:
	void Clear_BG(HDC hdc);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	bool Get_Platform_Image_Storke_Color(int x, int y, int &stroke_len, const AColor **color);
	void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
	void Draw_Expandig_Roll_In_State(HDC hdc, RECT paint_area);
	bool Reflect_On_Circle(double next_x_pos, double next_y_pos, ABall *ball, double x_offset = 0);

	EPlatform_State Platform_State;

	AColor Platform_Inner_Color, Platform_Circle_Color, Highlight_Color;

	RECT Prev_Platform_Rect, Platform_Rect;

	int Rolling_Step;

	static const int Circle_Size = 7;
	static const int Roll_In_Platform_End_X_Pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset) / 2 - Circle_Size / 2;
	static const int Rolling_Platform_Speed = 3;

	static const int Max_Rolling_Step = 16;
	static const int Inner_Height = 5;
	static const int Height = 7;
	static const int Meltdown_Speed = 4;
	static const int Normal_Inner_Width = 20;

	static const int Normal_Width = 28 * AsConfig::Global_Scale;
	static int Meltdown_Platform_Y_Pos[Normal_Width];

	int Normal_Platform_Image_Width;
	int Normal_Platform_Image_Height;
	int* Normal_Platform_Image;
};
//------------------------------------------------------------------------------------------------------------