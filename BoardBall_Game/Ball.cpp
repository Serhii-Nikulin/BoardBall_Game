#include "Ball.h"

const double ABall::Radius = 2;
int ABall::Counter_Hit_Checker = 0;
AHit_Checker *ABall::Hit_Checkers[Hit_Checkers_Count] = {};
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_State(EBS_Normal),
	Ball_Pen{}, Ball_Brush{}, Ball_Rect{}, Prev_Ball_Rect{},
	Center_X_Pos(0.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Ball_Direction(0.0), Rest_Distance(0.0)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Add_Hit_Checker(AHit_Checker *hit_checker)
{
	if (Counter_Hit_Checker < Hit_Checkers_Count)
		Hit_Checkers[Counter_Hit_Checker++] = hit_checker;
}
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
void ABall::Move()
{
	int i;
	double next_x_pos, next_y_pos;
	bool got_hit;
	double step_size = 1.0 / AsConfig::Global_Scale;
	Rest_Distance += Ball_Speed;
	Prev_Ball_Rect = Ball_Rect;

	if (Ball_State != EBS_Normal)
		return;

	while (Rest_Distance > 0)
	{
		got_hit = false;

		next_x_pos = Center_X_Pos + step_size * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - step_size * sin(Ball_Direction);

		for (i = 0; i < Hit_Checkers_Count; ++i)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);

		if (!got_hit)
		{
			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;
			Rest_Distance -= step_size;
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
		Center_X_Pos = 103;
		Center_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI_4;
		break;

	case EBS_Normal:
		Center_X_Pos = 103;
		Center_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
		Ball_Speed = AsConfig::Global_Scale * 2;
		Ball_Direction = M_PI_4;
		Rest_Distance = 0.0;
		break;

	case EBS_Lost:
		Ball_Speed = 0.0;
		break;	
	}

	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
double ABall::Get_Direction()
{
	return Ball_Direction;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_Direction(double direction)
{
	const double pi_mult_2 = M_PI * 2;

	while (direction > pi_mult_2)
		direction -= pi_mult_2;

	while (direction < 0.0)
		direction += pi_mult_2;

	Ball_Direction = direction;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Reflect(bool from_horizontal)
{
	if (from_horizontal)
		Set_Direction(-Ball_Direction);
	else
		Set_Direction(M_PI - Ball_Direction);
}
//------------------------------------------------------------------------------------------------------------
