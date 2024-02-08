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
