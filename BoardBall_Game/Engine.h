#pragma once
#include "Border.h"
#include "Ball_Set.h"
//#include "Ball.h"
#include "Platform.h"
#include "Level.h"

enum EGame_State
{
	EGS_Test_Ball,
	EGS_Play_Level, EGS_Lost_Ball, EGS_Restart_Level
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	enum EKey_Type
	{
		EKT_Left, EKT_Right, EKT_Space
	};

	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();
	void Play_Level();

private:
	void Act();
	void On_Falling_Letter(AFalling_Letter *falling_Letter);
	void Shift_Movers();

	EGame_State Game_State;
	AsLevel Level;
	AsBorder Border;
	AsPlatform Platform;
	AsBall_Set Ball_Set;
	AsLaser_Beam_Set Laser_Beam_Set;
	AMover *Movers[AsConfig::Max_Movers_Count];
	AGraphics_Object *Modules[AsConfig::Max_Modules_Count];
	double Rest_Distance;
	int Life_Count;
};
//------------------------------------------------------------------------------------------------------------


