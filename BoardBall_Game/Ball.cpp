#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_State(EBS_Normal),
	Ball_Pen{}, Ball_Brush{}, Ball_Rect{}, Prev_Ball_Rect{},
	Ball_X_Pos(0.0), Ball_Y_Pos(0.0), Ball_Speed(0.0), Ball_Direction(0.0)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
	AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);
		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
	{
		SelectObject(hdc, Ball_Pen);
		SelectObject(hdc, Ball_Brush);
		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(ALevel *level, int platform_x_pos, int platform_width)
{
	double next_x_pos, next_y_pos;

	int max_x_pos = AsConfig::Max_X_Pos - Ball_Size;
	int max_y_pos = AsConfig::Max_Y_Pos - Ball_Size;
	int platform_y_pos = AsConfig::Platform_Y_Pos - Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	if (Ball_State != EBS_Normal)
		return;

	next_x_pos = Ball_X_Pos + Ball_Speed * cos(Ball_Direction);
	next_y_pos = Ball_Y_Pos - Ball_Speed * sin(Ball_Direction);

	if (next_x_pos < AsConfig::Border_X_Offset)
	{
		next_x_pos = AsConfig::Border_X_Offset + (AsConfig::Border_X_Offset - next_x_pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos < AsConfig::Border_Y_Offset)
	{
		next_y_pos = AsConfig::Border_Y_Offset + (AsConfig::Border_Y_Offset - next_y_pos);
		Ball_Direction = -Ball_Direction;
	}

	if (next_x_pos > max_x_pos)
	{
		Ball_X_Pos = AsConfig::Max_X_Pos - Ball_Size;
		//next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
		Ball_Direction = -Ball_Direction + M_PI;
	}

	if (next_y_pos > max_y_pos)
	{
		if (level->Has_Floor)
		{
			Ball_Y_Pos = AsConfig::Max_Y_Pos - Ball_Size;
			//next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
			Ball_Direction = -Ball_Direction;
		}
		else
		{
			if (Ball_Y_Pos > AsConfig::Max_Y_Pos)
			{
				Ball_State = EBS_Lost;
				return;
			}
		}
	}

	if (next_y_pos > platform_y_pos)
		if (next_x_pos >= platform_x_pos - Ball_Size
			and 
			next_x_pos <= platform_x_pos + platform_width)
		{
			Ball_Direction = -Ball_Direction;
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
		}

	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)Ball_X_Pos * AsConfig::Global_Scale;
	Ball_Rect.top = (int)Ball_Y_Pos * AsConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale;

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state)
{
	switch (new_state)
	{
	case EBS_On_Platform:
		Ball_X_Pos = 103 - Ball_Size / 2 + 1;
		Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - Ball_Size;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		break;

	case EBS_Normal:
		Ball_X_Pos = 103 - Ball_Size / 2;
		Ball_Y_Pos = AsConfig::Platform_Y_Pos + 1 - Ball_Size;
		Ball_Speed = AsConfig::Global_Scale * 2;
		Ball_Direction = M_PI - M_PI_4;
		break;

	case EBS_Lost:
		Ball_Speed = 0.0;
		break;	
	}

	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
