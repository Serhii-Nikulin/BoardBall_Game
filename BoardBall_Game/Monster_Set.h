#pragma once
#include "Border.h"
#include "Monster.h"

//------------------------------------------------------------------------------------------------------------
enum class EMonster_Set_State: unsigned char
{
	Idle,
	Selecting_Next_Gate,
	Waiting_Gate_Opening,
	Waiting_Gate_Closing
};
//------------------------------------------------------------------------------------------------------------
class AsMonster_Set: public AGame_Objects_Set, public AHit_Checker
{
public:
	AsMonster_Set();

	virtual void Act();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball) ; // check hit with ball
	virtual bool Check_Hit(double next_x_pos, double next_y_pos); // check hit with laser_beams
	virtual bool Check_Hit(RECT &rect);

	void Init(AsBorder *border);
	void Emit_From_Gate(int gate_index);
	void Activate(int max_alive_monsters_count);
	void Destroy_All();

private:
	bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);
	static const int Max_Monsters_Count = 10;

	AMonster Monsters[Max_Monsters_Count];

	EMonster_Set_State Monster_Set_State;

	AsBorder *Border; // UNO

	int Current_Gate_Index;
	int Max_Alive_Monsters_Count;
};
//------------------------------------------------------------------------------------------------------------
