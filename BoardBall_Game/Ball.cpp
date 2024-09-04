#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
//AGraphics_Object
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
//AMover
AMover::~AMover()
{
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
//AHit_Checker
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
ABall::ABall(): Ball_State(EBS_Disabled), Prev_Ball_State(EBS_Disabled), Ball_Rect{}, Prev_Ball_Rect{}, Parachute_Rect{}, Prev_Parachute_Rect{},
	Center_X_Pos(0.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Ball_Direction(0.0), Time_Of_Release(0), Test_Iteration(0), Rest_Test_Distance(0.0), Testing_Is_Active(false)
{}
//------------------------------------------------------------------------------------------------------------
void ABall::Add_Hit_Checker(AHit_Checker *hit_checker)
{
	if (Counter_Hit_Checker < Hit_Checkers_Count)
		Hit_Checkers[Counter_Hit_Checker++] = hit_checker;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Begin_Movement()
{
	Prev_Ball_Rect = Ball_Rect;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Finish_Movement()
{
	if (Ball_State == EBS_Disabled or Ball_State == EBS_Lost)
		return;

	Redraw_Ball();

	if (Ball_State == EBS_On_Parachute)
	{
		Prev_Parachute_Rect = Parachute_Rect;

		Parachute_Rect.bottom = Ball_Rect.bottom;
		Parachute_Rect.top = Parachute_Rect.bottom - Parachute_Size * AsConfig::Global_Scale;

		Redraw_Parachute();
	}
}
//------------------------------------------------------------------------------------------------------------
double ABall::Get_Speed()
{
	return Ball_Speed;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Act()
{//code stub
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (Ball_State == EBS_Disabled)
		return;

	if ((Ball_State == EBS_Teleporting or Ball_State == EBS_Lost) and Ball_State == Prev_Ball_State)
		return;

	switch (Ball_State)
	{
	case EBS_On_Parachute:
		Draw_Parachute(hdc, paint_area);
		break;

	case EBS_Off_Parachute:
		Clear_Parachute(hdc);
		Set_State(EBS_Normal, Center_X_Pos, Center_Y_Pos, Ball_Direction);
		break;

	case EBS_Lost:
		if (Prev_Ball_State == EBS_On_Parachute)
			Clear_Parachute(hdc);	
		return;

	case EBS_Teleporting:
		return;
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
	{
		AsConfig::White_Color.Select(hdc);
		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if ((Ball_State == EBS_Teleporting or Ball_State == EBS_Lost) and Ball_State == Prev_Ball_State)
		return;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
	{
		AsConfig::BG_Color.Select(hdc);
		Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
bool ABall::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_Speed(double speed)
{
	Ball_Speed = speed;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Shift_Per_Step(double max_speed)
{
	int i;
	double next_x_pos, next_y_pos;
	double next_step;
	bool got_hit = true;
	int prev_hits_count = 0;
	int max_hits_count = 10;

	if (Ball_State == EBS_Disabled or Ball_State == EBS_Lost or Ball_State == EBS_Teleporting or Ball_State == EBS_On_Platform)
		return;

	Prev_Ball_Rect = Ball_Rect;

	next_step = AsConfig::Moving_Step_Size * Ball_Speed / max_speed;

	while (got_hit)
	{
		got_hit = false;

		next_x_pos = Center_X_Pos + next_step * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - next_step * sin(Ball_Direction);

		for (i = 0; i < Hit_Checkers_Count; ++i)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);

		if (got_hit)
		{
			++prev_hits_count;

			if (prev_hits_count >= 10)
			{
				Ball_Direction += M_PI / 8.0;
				prev_hits_count = 0;
			}
		}
		else
		{
			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;

			if (Testing_Is_Active)
				Rest_Test_Distance -= next_step;
		}

		/*if (Ball_State == EBS_On_Platform)
			break;*/
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.right = (int)((Center_X_Pos + Radius) * AsConfig::Global_Scale) - 1;
	Ball_Rect.bottom = (int)((Center_Y_Pos + Radius) * AsConfig::Global_Scale) - 1;

	AsConfig::Invalidate_Rect(Prev_Ball_Rect);
	AsConfig::Invalidate_Rect(Ball_Rect);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Parachute()
{
	AsConfig::Invalidate_Rect(Prev_Parachute_Rect);
	AsConfig::Invalidate_Rect(Parachute_Rect);
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
void ABall::Set_State(EBall_State new_state, double x_pos, double y_pos, double direction)
{
	switch (new_state)
	{
	case EBS_Disabled:
		Ball_Speed = 0.0;

	case EBS_On_Platform:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 0.0;
		Ball_Direction = direction;
		Time_Of_Release = AsConfig::Current_Timer_Tick + Time_On_Platform;
		break;

	case EBS_Normal:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = AsConfig::Ball_Normal_Speed;
		Ball_Direction = direction;
		break;
	
	case EBS_On_Parachute:
		AsConfig::Throw();
		break;

	case EBS_Off_Parachute:
		if (Ball_State != EBS_On_Parachute)
			AsConfig::Throw();

		Ball_Direction = M_PI_4 + 2.0 * M_PI_4 * AsConfig::Rand(2);
		Redraw_Ball();
		Redraw_Parachute();
		break;

	case EBS_Teleporting:
		if (! (Ball_State == EBS_Normal or Ball_State == EBS_On_Parachute or Ball_State == EBS_Teleporting) )
			AsConfig::Throw();

		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 0.0;

		Redraw_Ball();

		if (Ball_State == EBS_On_Parachute)
			Redraw_Parachute();

		break;

	case EBS_Lost:
		if (! (Ball_State == EBS_Normal or Ball_State == EBS_On_Parachute or Ball_State == EBS_Lost) )
			AsConfig::Throw();

		Redraw_Parachute();

		Ball_Speed = 0.0;
		break;	

	default:
		AsConfig::Throw();
	}

	Prev_Ball_State = Ball_State;
	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Get_Center(double &x_pos, double &y_pos)
{
	x_pos = Center_X_Pos;
	y_pos = Center_Y_Pos;
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
void ABall::Set_On_Parachute(int level_x, int level_y)
{
	int cell_x = AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width;
	int cell_y = AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height;

	Ball_State = EBS_On_Parachute;
	Ball_Direction = M_PI + M_PI_2;
	Ball_Speed = 1.0;

	Parachute_Rect.left = cell_x * AsConfig::Global_Scale;
	Parachute_Rect.top = cell_y * AsConfig::Global_Scale;
	Parachute_Rect.right = Parachute_Rect.left + Parachute_Size * AsConfig::Global_Scale - 1;
	Parachute_Rect.bottom = Parachute_Rect.top + Parachute_Size * AsConfig::Global_Scale - 1;

	Prev_Parachute_Rect = Parachute_Rect;

	Center_X_Pos = (double)(cell_x + AsConfig::Cell_Width / 2.0);
	Center_Y_Pos = (double)(cell_y + Parachute_Size);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw_Parachute(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;
	const int scale = AsConfig::Global_Scale;
	const int dome_size = Parachute_Size * scale / 2.0;
	int ball_center_x, ball_center_y;

	if (! IntersectRect(&intersection_rect, &paint_area, &Parachute_Rect) )
		return;

	Clear_Parachute(hdc);

	AsConfig::Blue_Color.Select(hdc);

	Chord(hdc, Parachute_Rect.left, Parachute_Rect.top, Parachute_Rect.right, Parachute_Rect.bottom, Parachute_Rect.right, Parachute_Rect.top + dome_size, Parachute_Rect.left, Parachute_Rect.top + dome_size);

	AsConfig::BG_Color.Select(hdc);

	Ellipse(hdc, Parachute_Rect.left + 0 * scale + 1, Parachute_Rect.top + 5 * scale + 1, Parachute_Rect.left + 4 * scale, Parachute_Rect.top + 9 * scale - 1);

	Ellipse(hdc, Parachute_Rect.left + 5 * scale - 1, Parachute_Rect.top + 5 * scale, Parachute_Rect.left + 10 * scale + 1, Parachute_Rect.top + 9 * scale);

	Ellipse(hdc, Parachute_Rect.left + 11 * scale, Parachute_Rect.top + 5 * scale + 1, Parachute_Rect.left + 14 * scale + 1, Parachute_Rect.top + 9 * scale - 1);

	ball_center_x = Parachute_Rect.left + Parachute_Size * scale / 2.0;
	ball_center_y = Parachute_Rect.bottom;

	AsConfig::White_Color.Select(hdc);

	MoveToEx(hdc, Parachute_Rect.left, Parachute_Rect.top + dome_size, NULL);
	LineTo(hdc, ball_center_x, ball_center_y - 1);

	MoveToEx(hdc, Parachute_Rect.left + 5 * scale - 1, Parachute_Rect.top + dome_size, NULL);
	LineTo(hdc, ball_center_x, ball_center_y);

	MoveToEx(hdc, Parachute_Rect.left + 11 * scale - 1, Parachute_Rect.top + dome_size, NULL);
	LineTo(hdc, ball_center_x, ball_center_y);

	MoveToEx(hdc, Parachute_Rect.right - 1, Parachute_Rect.top + dome_size, NULL);
	LineTo(hdc, ball_center_x, ball_center_y);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw_Teleporting(HDC hdc, int step)
{
	int top_y = Ball_Rect.top + step / 2;
	int low_y = Ball_Rect.bottom - step / 2;
	
	if (top_y >= low_y)
		return;

	AsConfig::White_Color.Select(hdc);
	Ellipse(hdc, Ball_Rect.left, top_y, Ball_Rect.right - 1, low_y - 1);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Clear_Parachute(HDC hdc)
{
	AsConfig::BG_Color.Select(hdc);

	AsConfig::Round_Rect(hdc, Prev_Parachute_Rect);
	AsConfig::Round_Rect(hdc, Parachute_Rect);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Shift_With_Direction(double direction, double platform_speed, double max_speed)
{
	double prev_direction = Ball_Direction;
	double prev_speed = Ball_Speed;
	EBall_State prev_ball_state = Ball_State;

	/*if (platform_speed < 0.0)
		platform_speed *= -1;*/

	Ball_Direction = direction;
	Ball_Speed = platform_speed;
	Ball_State = EBS_Normal;

	Shift_Per_Step(max_speed);

	Ball_Direction = prev_direction;
	Ball_Speed = prev_speed;
	Ball_State = prev_ball_state;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Release()
{
	double speed = Ball_Speed;
	double direction = Ball_Direction;
	Set_State(EBS_Normal, Center_X_Pos, Center_Y_Pos, direction);
	Set_Speed(speed);
	Time_Of_Release = 0;
}
//------------------------------------------------------------------------------------------------------------