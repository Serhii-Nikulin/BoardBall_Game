#pragma once
#include "Border.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State: unsigned char
{
	Closed, 
	Opening, 
	Staring,
	Closing
};
//------------------------------------------------------------------------------------------------------------
class AMonster: public AGame_Object
{
public:
	AMonster();
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Activate(int x_pos, int y_pos);
	bool Is_Active;
private:
	int X_Pos, Y_Pos;
	double Cornea_Height;
	int Start_Blink_Timeout;

	static const int Width = 16, Height = 16;
	static const int Max_Cornea_Height = 11;

	RECT Monster_Rect;
	EEye_State Eye_State;

	static const int Blink_Stages_Count = 7;
	static const double Blink_Timeouts[Blink_Stages_Count];
	static const EEye_State Blink_States[Blink_Stages_Count];
	int Blink_Ticks[Blink_Stages_Count];
	int Total_Animation_Timeout;
};
//------------------------------------------------------------------------------------------------------------
class AsMonster_Set: public AsGame_Objects_Set
{
public:
	AsMonster_Set();
	void Init(AsBorder *border);
	void Emit_From_Gate(int gate_index);

private:
	bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);
	static const int Max_Monsters_Count = 10;
	AMonster Monsters[Max_Monsters_Count];

	AsBorder *Border;//UNO
};
//------------------------------------------------------------------------------------------------------------


