#pragma once
#include <Windows.h>
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State {
	EBS_Normal, EBS_Lost, EBS_On_Platform
};
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
	bool Hit_Circle_On_Line(double next_pos, double eval_dist, double radius, double min_value, double max_value);
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Move();
	void Redraw_Ball();

	EBall_State Get_State();
	void Set_State(EBall_State new_state, int x_pos = 103, int y_pos = AsConfig::Platform_Y_Pos - Radius, double direction = M_PI_4);
	void Set_For_Test();
	double Get_Direction();
	void Set_Direction(double direction);
	void Reflect(bool from_horizontal);
	bool Is_Test_Finished();
	bool Is_Moving_Up();
	bool Is_Moving_Left();

	static void Add_Hit_Checker(AHit_Checker *hit_checker);
	static const double Radius;
	double Ball_Speed;
	double prev_angle_to_normal = 0;
	double Rest_Test_Distance;
private:
	EBall_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Direction;
	bool Testing_Is_Active;
	double Rest_Distance;
	static int Counter_Hit_Checker;
	static const int Hit_Checkers_Count = 3;
	static AHit_Checker *Hit_Checkers[Hit_Checkers_Count];
	int Test_Iteration;

};
//------------------------------------------------------------------------------------------------------------