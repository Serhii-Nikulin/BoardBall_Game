#include "Ball_Set.h"

//AsBall_Set
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Shift_Per_Step(double max_speed)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Shift_Per_Step(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AsBall_Set::Get_Speed()
{
	int i;
	double current_speed, max_speed = 0;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_speed = Balls[i].Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Release_From_Platform()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		if (Balls[i].Get_State() == EBS_On_Platform)
			Balls[i].Set_State(EBS_Normal);
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_On_Platform()
{
	int i;

	for (i = 0; i < 1; i++)
		Balls[i].Set_State(EBS_On_Platform);

	for (NULL; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBS_Disabled);

	Balls[0].Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::All_Balls_Are_Lost()
{
	int i;
	int active_balls_count = 0;
	int lost_balls_count = 0;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		if (Balls[i].Get_State() == EBS_Disabled)
			continue;

		++active_balls_count;

		if (Balls[i].Get_State() == EBS_Lost)
			++lost_balls_count;
	}

	if (active_balls_count == lost_balls_count)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_For_Test()
{
	Balls[0].Set_For_Test();
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Is_Test_Finished()
{
	return Balls[0].Is_Test_Finished();
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Triple_Ball()
{
	int i;
	double current_x, current_y;
	double max_y_distance = 0, current_distance;

	ABall *current_ball, *remote_ball = 0;
	ABall *left_candidate = 0, *right_candidate = 0;
	double direction;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() == EBS_Disabled or current_ball->Get_State() == EBS_Lost)
			continue;

		current_ball->Get_Center(current_x, current_y);
		current_distance = AsConfig::Max_Y_Pos - current_y;

		if (current_distance > max_y_distance)
		{
			max_y_distance = current_distance;
			remote_ball = current_ball;
		}
	}

	if (remote_ball == 0)
		return;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() == EBS_Disabled or current_ball->Get_State() == EBS_Lost)
		{
			if (left_candidate == 0)
				left_candidate = current_ball;
			else
			{
				right_candidate = current_ball;
				break;
			}
		}
	}

	direction = remote_ball->Get_Direction();

	if (left_candidate != 0)
	{
		*left_candidate = *remote_ball;
		left_candidate->Set_Direction(direction - M_PI / 8.0);
	}

	if(right_candidate != 0)
	{
		*right_candidate = *remote_ball;
		right_candidate->Set_Direction(direction + M_PI / 8.0);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Inverse_Direction()
{
	int i;
	ABall *current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() != EBS_Normal)
			continue;

		current_ball->Set_Direction(current_ball->Get_Direction() + M_PI);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Reset_Balls_Speed()
{
	int i = 0;

	ABall *current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];
		if (current_ball->Get_State() == EBS_Normal)
			current_ball->Set_Speed(AsConfig::Ball_Normal_Speed);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Accelerate()
{
	int i;
	ABall *current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() != EBS_Normal)
			continue;

		current_ball->Set_Speed(current_ball->Get_Speed() * AsConfig::Acceleration);
	}

}
//------------------------------------------------------------------------------------------------------------