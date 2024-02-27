#pragma once
#include "Active_Brick.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
	enum ELetter_Type {
		ELT_None, ELT_O
	};

public:
	ALevel();
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	static const int Level_Height = 14;
	static const int Level_Width = 12;

	static char Current_Level[Level_Height][Level_Width];
	static char Level_01[Level_Height][Level_Width];
	static char Test_Level[Level_Height][Level_Width];
	void Set_Current_Level(char level[Level_Height][Level_Width]);
	void Act();

	AActive_Brick Active_Brick;

private:
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);
	bool Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	void Add_Active_Brick(int level_x, int level_y);
	HPEN Brick_Blue_Pen;
	HBRUSH Brick_Blue_Brush;
	HPEN Brick_Red_Pen;
	HBRUSH Brick_Red_Brush;
	HPEN Letter_Pen;
	RECT Level_Rect;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	static const int Max_Active_Bricks_Count = 10;
	AActive_Brick *Active_Bricks[Max_Active_Bricks_Count];
	static int Active_Bricks_Count;
};
//------------------------------------------------------------------------------------------------------------