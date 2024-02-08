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
class AsEngine;
class ABall
{
public:
	ABall();
	void Ball_Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Move_Ball(AsEngine *engine);

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Ball_Direction;
	static const int Ball_Size = 4;

private:
	RECT Ball_Rect, Prev_Ball_Rect;
	int Ball_Speed;
	int Ball_X_Pos;
	int Ball_Y_Pos;
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	static const int Global_Scale = 3;

	static const int Level_Height = 14;
	static const int Level_Width = 12;

	static const int Border_X_Offset = 6;//Min_X
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;

	int Platform_X_Pos;
	int Platform_Width;
	static const int Platform_Y_Pos = 185;

	void Check_Level_Brick_Hit(int &next_y_pos);

private:
	void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);

	void Draw_Level(HDC hdc);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);

	void Draw_Platform(HDC hdc, int x, int y);
	void Redraw_Platform();

	void Draw_Bounds(HDC hdc, RECT& paint_area);
	void Draw_Border(HDC hdc, int x, int y, bool top_border);

	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);

	ABall Ball;

	//Config
	HPEN BG_Pen;
	HBRUSH BG_Brush;
	//Level
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
	//Platform
	HPEN Platform_Inner_Pen;
	HBRUSH Platform_Inner_Brush;
	HPEN Platform_Circle_Pen;
	HBRUSH Platform_Circle_Brush;
	HPEN Highlight_Pen;
	RECT Prev_Platform_Rect, Platform_Rect;
	int Inner_Width;
	static const int Inner_Height = 5;
	static const int Circle_Size = 7;
	static const int Platform_Height = 7;
	int Platform_X_Step;
	//Border
	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
	
};
//------------------------------------------------------------------------------------------------------------


