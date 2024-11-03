#pragma once
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBall_Set: public AGame_Objects_Set
{
public:
	virtual void Act();

	void Release_From_Platform();
	void Set_On_Platform();
	bool All_Balls_Are_Lost();
	void Set_For_Test();
	bool Is_Test_Finished();
	void Triple_Ball();
	void Inverse_Direction();
	void Reset_Balls_Speed();
	void Accelerate();

	bool Release_Next_Ball();
	void Shift_By_Platform(double direction, double platform_speed, double max_speed);

private:
	virtual bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);
	ABall Balls[AsConfig::Max_Balls_Count];

};
//------------------------------------------------------------------------------------------------------------