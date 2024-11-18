#pragma once
#include "Border.h"
#include "Ball_Set.h"
#include "Platform.h"
#include "Level.h"
#include "Monster_Set.h"

enum class EGame_State: unsigned char
{
	Test_Ball,
	Play_Level, 
	Lost_Ball, 
	Restart_Level
};
//------------------------------------------------------------------------------------------------------------
class AsInfo_Panel: public AGame_Object
{
public:
	~AsInfo_Panel();
	AsInfo_Panel();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

private:
	HFONT Logo_Font;

	void Choose_Font();
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	enum class EKey_Type: unsigned char
	{
		Left, 
		Right, 
		Space
	};

	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();
	void Play_Level();

	const int Timer_ID = WM_USER + 1;

private:
	void Restart_Level();
	void Act();
	void On_Falling_Letter(AFalling_Letter *falling_Letter);
	void Shift_Movers();
	void Add_Next_Module(int &index, AGame_Object *game_obj);

	EGame_State Game_State;
	AsLevel Level;
	AsBorder Border;
	AsPlatform Platform;
	AsBall_Set Ball_Set;
	AsLaser_Beam_Set Laser_Beam_Set;
	AsMonster_Set Monster_Set;

	AGame_Object *Modules[AsConfig::Max_Modules_Count];
	double Rest_Distance;
	int Life_Count;

	AsInfo_Panel Info_Panel;
};
//------------------------------------------------------------------------------------------------------------


