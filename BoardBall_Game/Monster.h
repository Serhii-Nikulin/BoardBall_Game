#pragma once
#include "Explosive_Ball.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed, 
	Opening, 
	Staring,
	Closing
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
