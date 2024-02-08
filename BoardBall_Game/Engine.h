#pragma once
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type {
	EBT_None, EBT_Red, EBT_Blue
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
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	const int Global_Scale = 3;

	HPEN BG_Pen;
	HBRUSH BG_Brush;

	HPEN Brick_Blue_Pen;
	HBRUSH Brick_Blue_Brush;
	HPEN Brick_Red_Pen;
	HBRUSH Brick_Red_Brush;
	HPEN Letter_Pen;
	RECT Level_Rect;
	const int Brick_Height = 7;
	const int Brick_Width = 15;
	const int Cell_Height = 8;
	const int Cell_Width = 16;
	const int Level_X_Offset = 8;
	const int Level_Y_Offset = 6;
	const static int Level_Height = 14;
	const static int Level_Width = 12;
	static char Level_01[Level_Height][Level_Width];

	HPEN Platform_Inner_Pen;
	HBRUSH Platform_Inner_Brush;
	HPEN Platform_Circle_Pen;
	HBRUSH Platform_Circle_Brush;
	HPEN Highlight_Pen;
	RECT Prev_Platform_Rect, Platform_Rect;
	const int Inner_Width = 20;
	const int Inner_Height = 5;
	int Platform_Width = 28;
	const int Circle_Size = 7;
	const int Platform_Height = 7;
	int Platform_X_Pos = 103 - Platform_Width / 2;
	const int Platform_Y_Pos = 185;
	int Platform_X_Step = 2 * Global_Scale;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	const int Ball_Size = 4;
	int Ball_X_Pos = Platform_X_Pos + (Platform_Width - Ball_Size) / 2;
	int Ball_Y_Pos = Platform_Y_Pos + 1 - Ball_Size;
	int Ball_Speed = Global_Scale;
	double Ball_Direction = M_PI - M_PI_4;

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
	const int Border_X_Offset = 6;//Min_X
	const int Max_X_Pos = 200;
	const int Border_Y_Offset = 4;//Min_Y
	const int Max_Y_Pos = 199;

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

	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, int rotation_step);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
};
//------------------------------------------------------------------------------------------------------------
