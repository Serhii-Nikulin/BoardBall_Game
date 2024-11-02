#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ABall: public AGame_Object, public ABall_Object
{
public:
	ABall();
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();
	virtual void Set_Speed(double speed);

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Ball();
	void Redraw_Parachute();

	virtual EBall_State Get_State();

	/*virtual void Set_State(EBall_State new_state, double x_pos = AsConfig::Start_Ball_Position_On_Platform, double y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Radius + 1 + 1.0 / AsConfig::Global_Scale, double direction = M_PI_4);*/

	virtual void Set_State(EBall_State new_state, double x_pos = Start_Ball_Position_On_Platform, double y_pos = Platform_Y_Pos - Ball_Radius + 1 + 1.0 / 3, double direction = M_PI_4);

	virtual void Get_Center(double &x_pos, double &y_pos);
	virtual bool Is_Moving_Up();
	virtual bool Is_Moving_Left();

	void Set_For_Test();

	virtual double Get_Direction();
	virtual void Set_Direction(double direction);

	virtual void Reflect(bool from_horizontal);
	bool Is_Test_Finished();
	virtual void Set_On_Parachute(int level_x, int level_y);

	virtual void Draw_Teleporting(HDC hdc, int step);

	void Shift_With_Direction(double direction, double platform_speed, double max_speed);
	void Release();

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