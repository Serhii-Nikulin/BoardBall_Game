#pragma once
#include <Windows.h>
#include "Level.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();
	void Init();
	void Draw(HDC hdc, RECT &paint_area);
	void Move(HWND hwnd, ALevel *level, int platform_x_pos, int platform_width);

private:
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;
	int Ball_X_Pos;
	int Ball_Y_Pos;
	int Ball_Speed;
	double Ball_Direction;
	static const int Ball_Size = 4;
};
//------------------------------------------------------------------------------------------------------------