#include "Engine.h"

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
	double x_max, y_max;
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
			/*x_max = current_x;
			y_max = current_y;*/
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
		//left_candidate->Set_State(EBS_Normal, x_max, y_max, direction - M_PI / 8.0);
	}

	if(right_candidate != 0)
	{
		*right_candidate = *remote_ball;
		right_candidate->Set_Direction(direction + M_PI / 8.0);
		//right_candidate->Set_State(EBS_Normal, x_max, y_max, direction + M_PI / 8.0);
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




//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Game_State(EGS_Lost_Ball), Rest_Distance(0.0)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	SYSTEMTIME sys_time;
	FILETIME file_time;
	
	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);
	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;
	ABall::Add_Hit_Checker(&Border);
	ABall::Add_Hit_Checker(&Level);
	ABall::Add_Hit_Checker(&Platform);

	AFalling_Letter::Init();
	Level.Init();
	Level.Set_Current_Level(AsLevel::Level_01);
	/*Ball.Set_State(EBS_Normal);
	Platform.Set_State(EPS_Normal); */
	AActive_Brick_Red_Blue::Setup_Colors();

	Platform.Redraw();
	
	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);

	memset(Movers, 0, sizeof(Movers) );

	Movers[0] = &Platform;
	Movers[1] = &Ball_Set;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	SetGraphicsMode(hdc, GM_ADVANCED);

	Border.Draw(hdc, paint_area);
	Level.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);
	Ball_Set.Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.Move(true, key_down);
		break;

	case EKT_Right:
		Platform.Move(false, key_down);
		break;

	case EKT_Space:
		if (key_down)
			if (Platform.Get_State() == EPS_Ready)
			{
				Ball_Set.Release_From_Platform();
				Platform.Set_State(EPS_Normal);
			}  

		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	AsConfig::Current_Timer_Tick += 1;

	switch (Game_State)
	{
	case EGS_Test_Ball:
		Ball_Set.Set_For_Test();
		Game_State = EGS_Play_Level;
		break;

	case EGS_Play_Level:
		Play_Level();
		break;

	case EGS_Lost_Ball:
		if (Platform.Get_State() == EPS_Missing)
		{
			Game_State = EGS_Restart_Level;
			Platform.Set_State(EPS_Roll_In);		
		}
		break;

	case EGS_Restart_Level:
		if (Platform.Get_State() == EPS_Ready)
		{
			Game_State = EGS_Play_Level;
			Ball_Set.Set_On_Platform();
		}

		break;
	}

	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	Shift_Mover();
	
	if (Ball_Set.All_Balls_Are_Lost() )
	{
		Game_State = EGS_Lost_Ball;
		Platform.Set_State(EPS_Meltdown);
		Level.Stop();
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished() )
		Game_State = EGS_Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;
	AFalling_Letter *falling_letter;

	Platform.Act();
	Level.Act();

	while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
	{
		if (Platform.Hit_By(falling_letter) )
			On_Falling_Letter(falling_letter);
	}

}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
		case ELT_O: 
			break;			
		case ELT_M: 
			break;
		case ELT_I: 
			Ball_Set.Inverse_Direction();
			break;
		case ELT_C:
			Ball_Set.Reset_Balls_Speed();
			break;
		case ELT_K: 
			break;
		case ELT_W: 
			break;				  
		case ELT_G:
			break;
		case ELT_T: 
			Ball_Set.Triple_Ball();
			break;
		case ELT_L: 
			break;
		case ELT_P: 
			break;
		case ELT_Plus:
			break;
		default:
			AsConfig::Throw();

	}

	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Shift_Mover()
{
	int i;
	double current_speed, max_speed = 0;
	
	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
	{
		if (Movers[i] != 0)
		{
			current_speed = fabs(Movers[i]->Get_Speed() );

			if (current_speed > max_speed)
				max_speed = current_speed;
		}
	}

	Rest_Distance += max_speed;

	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Movers[i] != 0)
			Movers[i]->Begin_Movement();

	while (Rest_Distance > 0.0)
	{
		for (i = 0; i < AsConfig::Max_Movers_Count; i++)
			if (Movers[i] != 0)
				Movers[i]->Shift_Per_Step(max_speed);

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}

	/*double x, y;
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		Ball_Set.Balls[i].Get_Center(x, y);

		if (x >= Platform.X_Pos and x < Platform.X_Pos + Platform.Width)
			if (y >= AsConfig::Platform_Y_Pos and y < AsConfig::Platform_Y_Pos + 7)
 				int yy = 0; 
	}*/


	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Movers[i] != 0)
			Movers[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------