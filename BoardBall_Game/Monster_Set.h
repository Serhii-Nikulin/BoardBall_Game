#pragma once
#include "Border.h"
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

	static const int Width = 16, Height = 16;
	static const int Max_Cornea_Height = 11;

	RECT Monster_Rect;
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


