#pragma once
#include <Windows.h>
#include "Level.h"
#include "Ball.h"

enum EBall_State {
	EBS_Normal, EBS_Lost, EBS_On_Platform
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Move(ALevel *level, int platform_x_pos, int platform_width);

	EBall_State Get_State();
private:
	EBall_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	double Ball_X_Pos;
	double Ball_Y_Pos;
	double Ball_Speed;
	double Ball_Direction;
	static const int Ball_Size = 4;
};
//------------------------------------------------------------------------------------------------------------