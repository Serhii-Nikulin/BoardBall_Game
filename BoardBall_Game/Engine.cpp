#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Hwnd{}
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	Hwnd = hwnd;
	Ball.Init();
	Level.Init();
	Platform.Init();
	Border.Init();
	AActive_Brick::Setup_Colors();
	AsConfig::Setup_Colors();

	Platform.Set_State(EPS_Roll_In);
	Platform.Redraw(Hwnd);
	SetTimer(Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	Level.Draw(Hwnd, hdc, paint_area);


	Ball.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);


	Border.Draw(hdc, paint_area);

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

		Platform.Redraw(Hwnd);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos > AsConfig::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw(Hwnd);
		break;

	case EKT_Space:

		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	AsConfig::Current_Timer_Tick += 1;

	Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);
	Level.Active_Brick.Act(Hwnd);

	//if (AsConfig::Current_Timer_Tick % 3 == 0)
	Platform.Act(Hwnd);

	return 0;
}
//------------------------------------------------------------------------------------------------------------
