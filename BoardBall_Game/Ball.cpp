#include "Ball.h"

//AHit_Checker
//------------------------------------------------------------------------------------------------------------
const double ABall::Radius = 2;
int ABall::Counter_Hit_Checker = 0;
AHit_Checker *ABall::Hit_Checkers[Hit_Checkers_Count] = {};
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double next_pos, double eval_dist, double radius, double min_value, double max_value)
{
	if (eval_dist > radius)
		return false;

	//value * value + eval_pos * eval_pos = radius * radius

	double value = sqrt(radius * radius - eval_dist * eval_dist);

	if ((next_pos + value) > min_value and (next_pos - value) < max_value)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------



//ABall
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_State(EBS_Normal),
	Ball_Pen{}, Ball_Brush{}, Ball_Rect{}, Prev_Ball_Rect{},
	Center_X_Pos(0.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Ball_Direction(0.0), Rest_Distance(0.0), 
	Test_Iteration(0), Rest_Test_Distance(0.0), Testing_Is_Active(false)
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
		Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
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
	Rest_Distance += Ball_Speed;
	Prev_Ball_Rect = Ball_Rect;

	if (Ball_State != EBS_Normal)
		return;

	while (Rest_Distance > 0)
	{
		got_hit = false;

		next_x_pos = Center_X_Pos + AsConfig::Moving_Step_Size * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - AsConfig::Moving_Step_Size * sin(Ball_Direction);

		for (i = 0; i < Hit_Checkers_Count; ++i)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);

		if (!got_hit)
		{
			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;
			Rest_Distance -= AsConfig::Moving_Step_Size ;

			if (Testing_Is_Active)
				Rest_Test_Distance -= AsConfig::Moving_Step_Size ;
		}
	}

	Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)(Center_X_Pos - Radius) * AsConfig::Global_Scale;
	Ball_Rect.top = (int)(Center_Y_Pos - Radius) * AsConfig::Global_Scale;
	Ball_Rect.right = (int)(Center_X_Pos + Radius) * AsConfig::Global_Scale - 1;
	Ball_Rect.bottom = (int)(Center_Y_Pos + Radius) * AsConfig::Global_Scale - 1;

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_For_Test()
{
	Testing_Is_Active = true;
	Rest_Test_Distance = 50;

	//Set_State(EBS_Normal, 102 + Test_Iteration, 65, M_PI + M_PI_4);//top -> left
	//Set_State(EBS_Normal, 62 + Test_Iteration, 100, M_PI_4);//low -> right
	//Set_State(EBS_Normal, 99 + Test_Iteration, 100, M_PI - M_PI_4);//low -> left
	//Set_State(EBS_Normal, 60 + Test_Iteration, 60, -M_PI_4);//top -> right
	 
	Set_State(EBS_Normal, 60 + Test_Iteration, 160, -M_PI_4);//ball reflects from ball of platform
	//Set_State(EBS_Normal, 85 + Test_Iteration, 160, -M_PI_2);//ball 
	//Set_State(EBS_Normal, 80, 197 - Test_Iteration, 0);//ball reflects from ball of platform
	//Set_State(EBS_Normal, 80 + Test_Iteration, 197, M_PI_4);//ball reflects from ball of platform


	++Test_Iteration;
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, int x_pos, int y_pos, double direction)
{
	switch (new_state)
	{
	case EBS_On_Platform:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 0.0;
		Ball_Direction = direction;
		break;

	case EBS_Normal:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = AsConfig::Global_Scale;
		Ball_Direction = direction;
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
	Set_Direction(Ball_Direction);
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
bool ABall::Is_Test_Finished()
{
	if (Testing_Is_Active)
		if (Rest_Test_Distance <= 0)
		{
			Testing_Is_Active = false;
			Set_State(EBS_Lost);
			return true;
		}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Up()
{
	if (Ball_Direction > 0 and Ball_Direction < M_PI)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Left()
{
	if (Ball_Direction > 0 and Ball_Direction < M_PI_2 or Ball_Direction > M_PI + M_PI_2 and Ball_Direction < 2 * M_PI)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
