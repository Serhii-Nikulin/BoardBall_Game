#pragma once
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBall_Set: public AMover, public AGraphics_Object
{
public:
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Release_From_Platform();
	void Set_On_Platform();
	bool All_Balls_Are_Lost();
	void Set_For_Test();
	bool Is_Test_Finished();
	void Triple_Ball();
	void Inverse_Direction();
	void Reset_Balls_Speed();
	void Accelerate();
private:
	ABall Balls[AsConfig::Max_Balls_Count];
};
//------------------------------------------------------------------------------------------------------------