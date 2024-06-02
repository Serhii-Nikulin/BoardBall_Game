#pragma once
#include "Border.h"
#include "Ball.h"
#include "Platform.h"
#include "Level.h"

enum EGame_State {
	EGS_Test_Ball,
	EGS_Play_Level, EGS_Lost_Ball, EGS_Restart_Level
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	enum EKey_Type {
		EKT_Left, EKT_Right, EKT_Space
	};

	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

private:
	void Act();
	void On_Falling_Letter(AFalling_Letter *falling_Letter);

	EGame_State Game_State;
	ABall Ball;
	AsLevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------


