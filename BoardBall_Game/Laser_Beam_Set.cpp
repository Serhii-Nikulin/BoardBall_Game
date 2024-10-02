#pragma once
#include "Laser_Beam_Set.h"

//AsLaser_Beam_Set
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
		Laser_Beams[i].Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
		Laser_Beams[i].Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Shift_Per_Step(double max_speed)
{
	int i;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
		Laser_Beams[i].Shift_Per_Step(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AsLaser_Beam_Set::Get_Speed()
{
	int i;
	double current_speed, max_speed = 0;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
	{
		current_speed = Laser_Beams[i].Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Act()
{
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
		Laser_Beams[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < Max_Laser_Beam_Count; i++)
		Laser_Beams[i].Clear_Prev_Animation(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsLaser_Beam_Set::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	int i;

	ALaser_Beam *left_beam = 0;
	ALaser_Beam *right_beam = 0;

	for (i = 0 ; i < Max_Laser_Beam_Count; i++)
	{
		if (Laser_Beams[i].Is_Active() )
			continue;

		if (left_beam == 0)
			left_beam = &Laser_Beams[i];
		else
			if (right_beam == 0)
			{
				right_beam = &Laser_Beams[i];
				break;
			}
	}

	if (left_beam == 0 or right_beam == 0)
		AsConfig::Throw();

	left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_Pos);
	right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_Pos);
}
//------------------------------------------------------------------------------------------------------------