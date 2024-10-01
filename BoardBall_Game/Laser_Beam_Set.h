#pragma once
#include "Laser_Beam.h"

//------------------------------------------------------------------------------------------------------------
class AsLaser_Beam_Set: public AMover, public AGraphics_Object
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

	void Fire(double left_gun_x_pos, double right_gun_x_pos);

private:
	static const int Max_Laser_Beam_Count = 10;
	ALaser_Beam Laser_Beams[Max_Laser_Beam_Count];

};
//------------------------------------------------------------------------------------------------------------
