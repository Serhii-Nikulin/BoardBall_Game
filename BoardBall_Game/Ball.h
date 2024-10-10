#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class EBall_State: unsigned char
{
	Disabled,

	Normal, 
	Lost, 
	On_Platform, 
	On_Parachute, 
	Off_Parachute, 
	Teleporting
};
//------------------------------------------------------------------------------------------------------------
class ABall: public AGame_Object
{
public:
	ABall();
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Set_Speed(double speed);
	void Redraw_Ball();
	void Redraw_Parachute();

	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos = AsConfig::Start_Ball_Position_On_Platform, double y_pos = AsConfig::Platform_Y_Pos - Radius + 1 + 1.0 / AsConfig::Global_Scale, double direction = M_PI_4);
	void Get_Center(double &x_pos, double &y_pos);
	void Set_For_Test();
	double Get_Direction();
	void Set_Direction(double direction);
	void Reflect(bool from_horizontal);
	bool Is_Test_Finished();
	bool Is_Moving_Up();
	bool Is_Moving_Left();
	void Set_On_Parachute(int level_x, int level_y);

	void Draw_Teleporting(HDC hdc, int step);

	void Shift_With_Direction(double direction, double platform_speed, double max_speed);
	void Release();

	static const double Radius;
	static const double Min_Ball_Direction;
	double prev_angle_to_normal = 0;
	double Rest_Test_Distance;

	int Time_Of_Release;

	static AHit_Checker_List Hit_Checker_List;

private:

	void Draw_Parachute(HDC hdc, RECT &paint_area);
	void Clear_Parachute(HDC hdc);

	EBall_State Ball_State, Prev_Ball_State;
	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect;


	double Ball_Speed;
	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Direction;
	bool Testing_Is_Active;
	static int Counter_Hit_Checker;
	static const int Parachute_Size = 15;
	static const int Time_On_Platform = 7 * AsConfig::FPS;
	int Test_Iteration;
};
//------------------------------------------------------------------------------------------------------------