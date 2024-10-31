#pragma once
#include "Border.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed, 
	Opening, 
	Staring,
	Closing
};
//------------------------------------------------------------------------------------------------------------
enum class EExplosive_Ball_State: unsigned char
{
	Idle,
	Charging,
	Expanding,
	Fading
};
//------------------------------------------------------------------------------------------------------------
enum class EMonster_State: unsigned char
{
	Missing,
	Emitting,
	Alive,
	Destroying
};
//------------------------------------------------------------------------------------------------------------
class AExplosive_Ball: public AGraphics_Object
{
public:
	AExplosive_Ball();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Explode(int x_pos, int y_pos, double max_size, bool is_red, int time_offset, int count_count);
	static void Setup_Colors();

private:
	void Update_Exploding_Ball_Rect();

	int X_Pos, Y_Pos;
	double Size, Max_Size;
	int Step_Count;
	int Time_Offset;
	double Size_Step;
	int Start_Fading_Tick;
	int Start_Expanding_Tick;
	bool Is_Red;
	static const int Fading_Timeout = AsConfig::FPS;
	static const int Max_Fade_Step = AsConfig::FPS;
	static AColor Fading_Red_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Colors[Max_Fade_Step];


	RECT Explosive_Ball_Rect;
	EExplosive_Ball_State Explosive_Ball_State;
};
//------------------------------------------------------------------------------------------------------------
class AMonster: public AGame_Object
{
public:
	AMonster();
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Activate(int x_pos, int y_pos, bool moving_right);
	bool Is_Active();
	void Destroy();

private:
	void Draw_Alive(HDC hdc);
	void Draw_Destroying(HDC hdc, RECT &paint_area);
	void Act_Alive();
	void Act_Destroying();
	void Redraw_Monster();

	double X_Pos, Y_Pos;
	double Cornea_Height;
	int Start_Blink_Timeout;

	double Speed;
	double Direction;

	int Direction_Switch_Tick;

	static const int Width = 16, Height = 16;
	static const int Max_Cornea_Height = 11;

	RECT Monster_Rect, Prev_Monster_Rect;
	EEye_State Eye_State;
	EMonster_State Monster_State;

	static const int Blink_Stages_Count = 7;
	static const double Blink_Timeouts[Blink_Stages_Count];
	static const EEye_State Blink_States[Blink_Stages_Count];
	int Blink_Ticks[Blink_Stages_Count];
	int Total_Animation_Timeout;

	int Alive_Timer_Tick;

	static const int Explosive_Balls_Count = 20;
	AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];
};
//------------------------------------------------------------------------------------------------------------
enum class EMonster_Set_State: unsigned char
{
	Idle,
	Selecting_Next_Gate,
	Waiting_Gate_Opening,
	Waiting_Gate_Closing
};
//------------------------------------------------------------------------------------------------------------
class AsMonster_Set: public AGame_Objects_Set
{
public:
	AsMonster_Set();
	void Act();
	void Init(AsBorder *border);
	void Emit_From_Gate(int gate_index);
	void Activate(int max_alive_monsters_count);
private:
	bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);
	static const int Max_Monsters_Count = 10;

	AMonster Monsters[Max_Monsters_Count];

	EMonster_Set_State Monster_Set_State;

	AsBorder *Border; // UNO

	int Current_Gate_Index;
	int Max_Alive_Monsters_Count;
};
//------------------------------------------------------------------------------------------------------------
