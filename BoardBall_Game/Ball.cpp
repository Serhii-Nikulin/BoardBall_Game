#include "Ball.h"

const double ABall::Radius = 2;
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_State(EBS_Normal),
	Ball_Pen{}, Ball_Brush{}, Ball_Rect{}, Prev_Ball_Rect{},
	Center_X_Pos(0.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Ball_Direction(0.0)
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
	bool got_hit;
	int platform_y_pos = AsConfig::Platform_Y_Pos - Radius + 1;
	double step_size = 1.0 / AsConfig::Global_Scale;
	double rest_distance = Ball_Speed;
	Prev_Ball_Rect = Ball_Rect;

	if (Ball_State != EBS_Normal)
		return;

	while (rest_distance >= step_size)
	{
		got_hit = false;

		next_x_pos = Center_X_Pos + step_size * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - step_size * sin(Ball_Direction);

		if (next_x_pos - Radius < AsConfig::Border_X_Offset)
		{
			got_hit = true;
			Ball_Direction = M_PI - Ball_Direction;
		}

		if (next_y_pos - Radius < AsConfig::Border_Y_Offset)
		{
			got_hit = true;
			Ball_Direction = -Ball_Direction;
		}

		if (next_x_pos + Radius > AsConfig::Max_X_Pos)
		{
			got_hit = true;
			Ball_Direction = -Ball_Direction + M_PI;
		}

		if (next_y_pos + Radius > AsConfig::Max_Y_Pos)
		{
			if (!level->Has_Floor)
			{
				got_hit = true;
				Ball_Direction = -Ball_Direction;
			}
			else
			{
				if (next_y_pos - Radius * 4 > AsConfig::Max_Y_Pos)
				{
					Ball_State = EBS_Lost;
					return;
				}
			}
		}

		if ((int)next_y_pos + Radius > platform_y_pos)
			if (next_x_pos >= platform_x_pos - Radius
				and 
				next_x_pos <= platform_x_pos + platform_width + Radius)
			{
				got_hit = true;
				Ball_Direction = -Ball_Direction;
			}

		//level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);
		
		if (!got_hit)
		{
			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;
			rest_distance -= step_size;
		}
	}

	Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)(Center_X_Pos - Radius) * AsConfig::Global_Scale;
	Ball_Rect.top = (int)(Center_Y_Pos - Radius) * AsConfig::Global_Scale;
	Ball_Rect.right = (int)(Center_X_Pos + Radius) * AsConfig::Global_Scale;
	Ball_Rect.bottom = (int)(Center_Y_Pos + Radius) * AsConfig::Global_Scale;

	//InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
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
		Center_X_Pos = 103;
		Center_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		break;

	case EBS_Normal:
		Center_X_Pos = 103;
		Center_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
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
