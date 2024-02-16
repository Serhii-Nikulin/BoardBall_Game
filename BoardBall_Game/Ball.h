#pragma once
#include <Windows.h>
#include "Level.h"
#include "Ball.h"

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
	void Move(int platform_x_pos, int platform_width, AHit_Checker *level);
	void Redraw_Ball();

	EBall_State Get_State();
	void Set_State(EBall_State new_state);

	double Ball_Direction;
	static const double Radius;

private:
	EBall_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	double Center_X_Pos;
	double Center_Y_Pos;
	double Ball_Speed;
};
//------------------------------------------------------------------------------------------------------------