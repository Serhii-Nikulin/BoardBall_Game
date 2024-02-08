#pragma once
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type {
	EBT_None, EBT_Red, EBT_Blue
};
enum ELetter_Type {
	ELT_None, ELT_O
};
enum EKey_Type {
	EKT_Left, EKT_Right, EKT_Space
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	static const int Level_Height = 14;
	static const int Level_Width = 12;

private:
	void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN& pen, HBRUSH& brush);

	void Draw_Level(HDC hdc);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);

	void Draw_Platform(HDC hdc, int x, int y);
	void Redraw_Platform();

	void Draw_Ball(HDC hdc, RECT& paint_area);
	void Move_Ball();
	void Check_Level_Brick_Hit(int& next_y_pos);

	void Draw_Bounds(HDC hdc, RECT& paint_area);
	void Draw_Border(HDC hdc, int x, int y, bool top_border);

	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);

	HWND Hwnd;
	static const int Global_Scale = 3;

	HPEN BG_Pen;
	HBRUSH BG_Brush;

	HPEN Brick_Blue_Pen;
	HBRUSH Brick_Blue_Brush;
	HPEN Brick_Red_Pen;
	HBRUSH Brick_Red_Brush;
	HPEN Letter_Pen;
	RECT Level_Rect;
	static const int Brick_Height = 7;
	static const int Brick_Width = 15;
	static const int Cell_Height = 8;
	static const int Cell_Width = 16;
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;

	HPEN Platform_Inner_Pen;
	HBRUSH Platform_Inner_Brush;
	HPEN Platform_Circle_Pen;
	HBRUSH Platform_Circle_Brush;
	HPEN Highlight_Pen;
	RECT Prev_Platform_Rect, Platform_Rect;
	int Inner_Width;
	static const int Inner_Height = 5;
	int Platform_Width;
	static const int Circle_Size = 7;
	static const int Platform_Height = 7;
	int Platform_X_Pos;
	static const int Platform_Y_Pos = 185;
	int Platform_X_Step;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	static const int Ball_Size = 4;
	int Ball_X_Pos;
	int Ball_Y_Pos;
	int Ball_Speed;
	double Ball_Direction;

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
	static const int Border_X_Offset = 6;//Min_X
	static const int Max_X_Pos = 200;
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_Y_Pos = 199;
};
//------------------------------------------------------------------------------------------------------------
