#pragma once
#include "Falling_Letter.h"
#include "Ball.h"

class AsLevel: public AHit_Checker
{
public:
	AsLevel();
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Drow_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_counter);
	bool Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter);

	static const int Level_Height = 14;
	static const int Level_Width = 12;

	static char Current_Level[Level_Height][Level_Width];
	static char Level_01[Level_Height][Level_Width];
	static char Test_Level[Level_Height][Level_Width];
	void Set_Current_Level(char level[Level_Height][Level_Width]);
	void Act();
	void Act_Objects(AGraphics_Object **objects_array, const int objects_max_count, int &object_count);

	AActive_Brick_Red_Blue Active_Brick;

private:
	void Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);
	bool Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball);
	void On_Hit(int level_x, int level_y);
	void Redraw_Brick(int level_x, int level_y);
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