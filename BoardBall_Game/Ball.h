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
	void Set_State(EBall_State new_state);
	double Get_Direction();
	void Set_Direction(double direction);
	void Reflect(bool from_horizontal);
	static void Add_Hit_Checker(AHit_Checker *hit_checker);
	static const double Radius;

private:
	EBall_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Speed;
	double Ball_Direction;
	double Rest_Distance;
	static int Counter_Hit_Checker;
	static const int Hit_Checkers_Count = 3;
	static AHit_Checker *Hit_Checkers[Hit_Checkers_Count];
};
//------------------------------------------------------------------------------------------------------------