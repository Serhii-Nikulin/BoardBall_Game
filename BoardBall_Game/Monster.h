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
class AMonster: public AGame_Object, public AHit_Checker
{
public:
	virtual ~AMonster();
	AMonster();
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball) ; // check hit with ball
	virtual bool Check_Hit(double next_x_pos, double next_y_pos); // check hit with laser_beams
	virtual bool Check_Hit(RECT &rect);

	void Activate(int x_pos, int y_pos, bool moving_right);
	bool Is_Active() const;
	void Destroy();

protected:
	virtual void Draw_Alive(HDC hdc) = 0;
	virtual void Act_Alive() = 0;
	virtual void On_Activation() = 0;

	EMonster_State Monster_State;
	RECT Monster_Rect, Prev_Monster_Rect;
	double Direction;
	int Direction_Switch_Tick; 
	static const int Width = 16, Height = 16;

private:
	void Change_Direction();

	void Draw_Destroying(HDC hdc, RECT &paint_area);
	void Act_Destroying();
	void Redraw_Monster();
	void Update_Rect(int x_pos, int y_pos, RECT &rect);
	double X_Pos, Y_Pos;
	double Speed;
	int Alive_Timer_Tick;
	static const int Explosive_Balls_Count = 20;
	AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];
};
//------------------------------------------------------------------------------------------------------------
class AMonster_Eye: public AMonster
{
public:
	AMonster_Eye();

private:
	virtual void Draw_Alive(HDC hdc);
	virtual void Act_Alive();
	virtual void On_Activation();

	EEye_State Eye_State;
	double Cornea_Height;

	int Start_Blink_Timeout;
	int Total_Animation_Timeout;

	static const int Blink_Stages_Count = 7;
	int Blink_Ticks[Blink_Stages_Count];
	static const double Blink_Timeouts[Blink_Stages_Count];
	static const EEye_State Blink_States[Blink_Stages_Count];
	static const int Max_Cornea_Height = 11;
};
//------------------------------------------------------------------------------------------------------------
class AMonster_Comet: public AMonster
{
public:
	AMonster_Comet();

private:
	virtual void Draw_Alive(HDC hdc) ;
	virtual void Act_Alive();
	virtual void On_Activation();

};
//------------------------------------------------------------------------------------------------------------

