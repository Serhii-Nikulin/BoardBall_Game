#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Game_State(EGS_Play_Level)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	AsConfig::Hwnd = hwnd;
	ABall::Add_Hit_Checker(&Border);
	ABall::Add_Hit_Checker(&Level);
	ABall::Add_Hit_Checker(&Platform);

	Ball.Init();
	Level.Init();
	Level.Set_Current_Level(ALevel::Level_01);
	Border.Init();
	Ball.Set_State(EBS_Normal);
	Platform.Set_State(EPS_Normal); 
	Platform.Init();
	AActive_Brick::Setup_Colors();
	AsConfig::Setup_Colors();

	Platform.Redraw();
	
	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	Ball.Draw(hdc, paint_area);
	Border.Draw(hdc, paint_area);
	Level.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);

	/*int i;
	for (i = 0; i < 16; ++i)
	{
		Level.Draw_Brick_Letter(hdc, (20 + i * ALevel::Cell_Width) * Global_Scale, 100, EBT_Blue, ELT_O, i);
		Level.Draw_Brick_Letter(hdc, (20 + i * ALevel::Cell_Width) * Global_Scale, 100 + 50, EBT_Red, ELT_O, i);
	}*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos < AsConfig::Border_X_Offset)
			Platform.X_Pos = AsConfig::Border_X_Offset;

		Platform.Redraw();
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos > AsConfig::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw();
		break;

	case EKT_Space:
		if (Platform.Get_State() == EPS_Ready)
		{
			Ball.Set_State(EBS_Normal);
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
		Ball.Set_For_Test();
		Game_State = EGS_Play_Level;
		break;

	case EGS_Play_Level:
		Ball.Move();

		if (Ball.Get_State() == EBS_Lost)
		{
			Game_State = EGS_Lost_Ball;
			Platform.Set_State(EPS_Meltdown);
		}

		if (Ball.Is_Test_Finished())
			Game_State = EGS_Test_Ball;

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
			Ball.Set_State(EBS_On_Platform);
			Ball.Redraw_Ball();
		}
		break;
	}

	Platform.Act();

	Level.Active_Brick.Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------