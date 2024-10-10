#include "Ball_Set.h"

//AsBall_Set
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Act()
{
	int i;
	ABall *current_ball = 0;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() == EBall_State::On_Platform)
			if (current_ball->Time_Of_Release != 0 and current_ball->Time_Of_Release <= AsConfig::Current_Timer_Tick)
				current_ball->Release();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Release_From_Platform()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		if (Balls[i].Get_State() == EBall_State::On_Platform)
			Balls[i].Set_State(EBall_State::Normal);
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_On_Platform()
{
	int i;

	for (i = 0; i < 1; i++)
		Balls[i].Set_State(EBall_State::On_Platform);

	for (NULL; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBall_State::Disabled);

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
		if (Balls[i].Get_State() == EBall_State::Disabled)
			continue;

		++active_balls_count;

		if (Balls[i].Get_State() == EBall_State::Lost)
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

		if (current_ball->Get_State() == EBall_State::Disabled or current_ball->Get_State() == EBall_State::Lost)
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

		if (current_ball->Get_State() == EBall_State::Disabled or current_ball->Get_State() == EBall_State::Lost)
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

		if (current_ball->Get_State() != EBall_State::Normal)
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
		if (current_ball->Get_State() == EBall_State::Normal)
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

		if (current_ball->Get_State() != EBall_State::Normal)
			continue;

		current_ball->Set_Speed(current_ball->Get_Speed() * AsConfig::Acceleration);
	}

}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Release_Next_Ball()
{
	int i;
	ABall *current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() == EBall_State::On_Platform)
		{
			current_ball->Release();
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Shift_By_Platform(double direction, double platform_speed, double max_speed)
{
	int i;
	ABall *current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() == EBall_State::On_Platform)
		{
			current_ball->Shift_With_Direction(direction, platform_speed, max_speed);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Get_Next_Game_Object(int &index, AGame_Object **game_obj)
{
	if (index < 0 or index >= AsConfig::Max_Balls_Count)
		return false;

	*game_obj = &Balls[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------