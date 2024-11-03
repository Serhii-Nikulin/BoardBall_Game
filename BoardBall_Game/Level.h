#pragma once
#include "Falling_Letter.h"

//------------------------------------------------------------------------------------------------------------
struct SPoint
{
	int X, Y;
};
//------------------------------------------------------------------------------------------------------------
class AsLevel: public AHit_Checker, public AGame_Object
{
public:
	~AsLevel();
	AsLevel();
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static bool Has_Brick_At_Pos(int level_x_pos, int level_y_pos);
	static bool Has_Brick_At_Rect(RECT rect);
	void Init();
	void Clear_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_counter);
	void Drow_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_counter);
	bool Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter);

	static const int Level_Height = 14;
	static const int Level_Width = 12;

	char Current_Level[Level_Height][Level_Width];
	static char Level_01[Level_Height][Level_Width];

	static char Test_Level[Level_Height][Level_Width];
	void Set_Current_Level(char level[Level_Height][Level_Width]);
	void Act_Objects(AGraphics_Object **objects_array, const int objects_max_count, int &object_count);
	void Stop();

	AActive_Brick_Red_Blue Active_Brick;
	bool Should_Stop_Level;

private:
	void Draw_Brick(HDC hdc, RECT &brick_rect, int level_x, int level_y);
	bool Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object *ball);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object *ball);
	void On_Hit(int level_x, int level_y, ABall_Object *ball, bool vertical_hit);
	void Redraw_Brick(int level_x, int level_y);
	void Create_Active_Brick(int level_x, int level_y, EBrick_Type brick_type, ABall_Object *ball, bool vertical_hit);
	void Add_Active_Brick_Teleport(int level_x, int level_y, ABall_Object *ball, bool vertical_hit);
	bool Add_Falling_Letter(int level_x, int level_y, EBrick_Type brick_type);
	void Add_New_Active_Brick(AActive_Brick *active_brick);
	AActive_Brick_Teleport *Select_Destination_Teleport(int source_x, int source_y);
	void Draw_Parachute_In_Level(HDC hdc, RECT brick_rect);
	void Clear_All_Activity();
	void Delete_Objects(AGraphics_Object **objects_array, const int objects_max_count, int &object_count);

	int Teleport_Bricks_Count;
	SPoint *Teleport_Bricks_Pos;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	RECT Level_Rect;

	static const int Max_Active_Bricks_Count = 20;
	AActive_Brick *Active_Bricks[Max_Active_Bricks_Count];
	static int Active_Bricks_Count;
	const AColor Parachute_Color;

	int Falling_Letters_Count = 0;
	AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letters_Count] = {};
	AAdvertisement *Advertisement;

	static AsLevel *Level;
};
//------------------------------------------------------------------------------------------------------------