#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Game_State(EGS_Lost_Ball)
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
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	Border.Draw(hdc, paint_area);
	Level.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	int i;

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
				for (i = 0; i < AsConfig::Max_Balls_Count; i++)
					if (Balls[i].Get_State() == EBS_On_Platform)
						Balls[i].Set_State(EBS_Normal);

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
		Balls[0].Set_For_Test();
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
			Restart_Level();
			
		break;
	}

	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
	int i;

	Game_State = EGS_Play_Level;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBS_On_Platform);

	for (NULL; i < AsConfig::Max_Balls_Count; i++)
		//Balls[i].Set_State(EBS_Disabled);

	Balls[0].Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	int i;
	int active_balls_count = 0;
	int lost_balls_count = 0;
	double max_speed = fabs(Platform.Speed);
	double rest_distance = max_speed;

	while (rest_distance > 0.0)
	{
		Platform.Shift_Per_Step(max_speed);
		rest_distance -= AsConfig::Moving_Step_Size;
	}

	Platform.Redraw();
	
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		if (Balls[i].Get_State() == EBS_Disabled)
			continue;

		++active_balls_count;

		if (Balls[i].Get_State() == EBS_Lost)
			++lost_balls_count;

		Balls[i].Move();

		double x, y;

		Balls[i].Get_Center(x, y);

		/*if (x >= Platform.X_Pos and x < Platform.X_Pos + Platform.Width)
			if (y >= AsConfig::Platform_Y_Pos and y < AsConfig::Platform_Y_Pos + 7)
				int yy = 0; */
	}

	if (active_balls_count == lost_balls_count)
	{
		Game_State = EGS_Lost_Ball;
		Platform.Set_State(EPS_Meltdown);
		Level.Stop();
	}

	if (active_balls_count == 1)
		if (Balls[0].Is_Test_Finished() )
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
	/*switch (falling_letter->Letter_Type)
	{
		case ELT_O: 
			break;			
		case ELT_M: 
			break;
		case ELT_I: 
			break;
		case ELT_C: 
			break;
		case ELT_K: 
			break;
		case ELT_W: 
			break;				  
		case ELT_G:
			break;
		case ELT_T: 
			break;
		case ELT_L: 
			break;
		case ELT_P: 
			break;
		case ELT_Plus:
			break;
		default:
			AsConfig::Throw();

	}*/

	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------