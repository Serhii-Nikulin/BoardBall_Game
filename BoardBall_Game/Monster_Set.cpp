#include "Monster_Set.h"

//AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
: Border(0), Monsters{}, Monster_Set_State(EMonster_Set_State::Idle), Current_Gate_Index(-1), Max_Alive_Monsters_Count(0)
{
	memset(Monsters, 0, sizeof(Monsters[0]) * Max_Monsters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Act()
{
	int i;
	int current_alive_monsters_count = 0;

	switch (Monster_Set_State)
	{
	case EMonster_Set_State::Idle:
		break;

	case EMonster_Set_State::Selecting_Next_Gate:
		for (i = 0; i < Max_Monsters_Count; i++)
			if (Monsters[i] != 0 and Monsters[i]->Is_Active() )
				current_alive_monsters_count += 1;

		if (current_alive_monsters_count < Max_Alive_Monsters_Count)
		{
			Current_Gate_Index = Border->Long_Open_Gate();
			Monster_Set_State = EMonster_Set_State::Waiting_Gate_Opening;
		}
		break;

	case EMonster_Set_State::Waiting_Gate_Opening:
		if (Border->Is_Gate_Opened(Current_Gate_Index) )
		{
			Emit_From_Gate(Current_Gate_Index);
			Monster_Set_State = EMonster_Set_State::Waiting_Gate_Closing;
		}
		break;

	case EMonster_Set_State::Waiting_Gate_Closing:
		if (Border->Is_Gate_Closed(Current_Gate_Index) )
		{
			Monster_Set_State = EMonster_Set_State::Selecting_Next_Gate;
		}
		break;

	default:
		AsConfig::Throw();
	}

	if (Monster_Set_State != EMonster_Set_State::Idle)
	{
		for (i = 0; i < Max_Monsters_Count; i++)
		{
			if (Monsters[i] != 0 and Monsters[i]->Is_Finished() )
			{
				delete Monsters[i];
				Monsters[i] = 0;
			}
		}
	}

	AGame_Objects_Set::Act();
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball)
{
	int i;

	for (i = 0; i < Max_Monsters_Count; i++)
		if (Monsters[i] != 0 and Monsters[i]->Check_Hit(next_x_pos, next_y_pos, ball) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(double next_x_pos, double next_y_pos)
{
	int i;

	for (i = 0; i < Max_Monsters_Count; i++)
		if (Monsters[i] != 0 and Monsters[i]->Check_Hit(next_x_pos, next_y_pos) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(RECT &rect)
{
	int i;

	for (i = 0; i < Max_Monsters_Count; i++)
		if (Monsters[i] != 0 and Monsters[i]->Check_Hit(rect) )
			return true;
	
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder *border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_From_Gate(int gate_index)
{
	int i;
	int gate_x_pos, gate_y_pos;
	bool gate_is_left;

	AMonster *monster = 0;

	if (gate_index < 0 or gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (Monsters[i] == 0)
		{
			monster = new AMonster_Eye();
			Monsters[i] = monster;
			break;
		}
	}

	if (monster == 0)
		return;

	if (gate_index % 2 == 0)
		gate_is_left = true;
	else
		gate_is_left = false;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);
	monster->Activate(gate_x_pos, gate_y_pos + 1, gate_is_left);
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Activate(int max_alive_monsters_count)
{
	Monster_Set_State = EMonster_Set_State::Selecting_Next_Gate;
	Max_Alive_Monsters_Count = max_alive_monsters_count;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Destroy_All()
{
	int i;
	
	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (Monsters[i] != 0 and Monsters[i]->Is_Active() )
			Monsters[i]->Destroy();
	}

	Monster_Set_State = EMonster_Set_State::Idle;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_Game_Object(int &index, AGame_Object **object)
{
	AMonster *monster;

	if (index < 0 or index >= Max_Monsters_Count)
		return false;

	while (index < Max_Monsters_Count)
	{
		monster = Monsters[index++];

		if (monster != 0)
		{
			*object = monster;
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------