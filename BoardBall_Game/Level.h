#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ALevel
{
	enum EBrick_Type {
		EBT_None, EBT_Red, EBT_Blue
	};
	enum ELetter_Type {
		ELT_None, ELT_O
	};

public:
	ALevel();
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

private:
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);

	HPEN Brick_Blue_Pen;
	HBRUSH Brick_Blue_Brush;
	HPEN Brick_Red_Pen;
	HBRUSH Brick_Red_Brush;
	HPEN Letter_Pen;
	RECT Level_Rect;

	static const int Level_Height = 14;
	static const int Level_Width = 12;
	static char Level_01[ALevel::Level_Height][ALevel::Level_Width];
	static const int Cell_Width = 16;
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Height = 8;
};
//------------------------------------------------------------------------------------------------------------