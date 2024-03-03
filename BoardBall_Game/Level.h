#pragma once
#include "Active_Brick.h"
#include "Ball.h"

enum ELetter_Type {
	ELT_None, ELT_O
};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter
{
	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);

	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;
	int Rotation_Step;
	int X, Y;


public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);
	RECT Letter_Cell, Prev_Letter_Cell;

	void Act();
	void Draw(HDC hdc);
	bool Is_Finished();
};
//------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
public:
	ALevel();
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	void Init();
	void Draw(HDC hdc, RECT &paint_area);

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
	bool Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	void On_Hit(int level_x, int level_y);
	void Add_Active_Brick(int level_x, int level_y, EBrick_Type brick_type);
	bool Add_Falling_Letter(int level_x, int level_y, EBrick_Type brick_type);

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	RECT Level_Rect;

	static const int Max_Active_Bricks_Count = 10;
	AActive_Brick *Active_Bricks[Max_Active_Bricks_Count];
	static int Active_Bricks_Count;

	int Falling_Letters_Count = 0;
	AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letters_Count] = {};
};
//------------------------------------------------------------------------------------------------------------