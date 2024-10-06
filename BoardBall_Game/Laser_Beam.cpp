#include "Laser_Beam.h"

//ALaser_Beam
//------------------------------------------------------------------------------------------------------------
AHit_Checker_List ALaser_Beam::Hit_Checker_List;
//------------------------------------------------------------------------------------------------------------
ALaser_Beam::ALaser_Beam()
	: X_Pos(0), Y_Pos(0), Speed(0.0), Laser_Rect{}, Prev_Laser_Rect{}, Laser_Beam_State(ELaser_Beam_State::Disabled)
{
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Begin_Movement()
{
	// code stub
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Finish_Movement()
{
	if (Laser_Beam_State != ELaser_Beam_State::Disabled)
		Redraw_Beam();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Shift_Per_Step(double max_speed)
{
	double next_step;

	if (Laser_Beam_State != ELaser_Beam_State::Active)
		return;

	next_step = AsConfig::Moving_Step_Size * Speed / max_speed;

	Y_Pos -= next_step;

	if (Hit_Checker_List.Check_Hit(X_Pos, Y_Pos) )
		Disable();

	if (Y_Pos < AsConfig::Level_Y_Offset)
		Disable();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Disable()
{
	Laser_Beam_State = ELaser_Beam_State::Stopping;
	Speed = 0.0;
}
//------------------------------------------------------------------------------------------------------------
double ALaser_Beam::Get_Speed()
{
	return Speed;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Act()
{
	// code stub
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;
	int x_pos, y_pos;

	if (Laser_Beam_State == ELaser_Beam_State::Disabled)
		return;

	if (Laser_Beam_State == ELaser_Beam_State::Stopping)
		Laser_Beam_State = ELaser_Beam_State::Cleanup;

	if (! IntersectRect(&intersection_rect, &Laser_Rect, &paint_area) )
		return;

	x_pos = (int)(X_Pos * AsConfig::D_Global_Scale);
	y_pos = (int)(Y_Pos * AsConfig::D_Global_Scale);

	AsConfig::Laser_Color.Select(hdc);

	MoveToEx(hdc, x_pos, y_pos, 0);
	LineTo(hdc, x_pos, y_pos + Height * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (Laser_Beam_State == ELaser_Beam_State::Disabled)
		return;

	if (Laser_Beam_State == ELaser_Beam_State::Cleanup)
		Laser_Beam_State = ELaser_Beam_State::Disabled;

	if (! IntersectRect(&intersection_rect, &Prev_Laser_Rect, &paint_area) )
		return;

	AsConfig::BG_Color.Select(hdc);

	Rectangle(hdc, Prev_Laser_Rect.left, Prev_Laser_Rect.top, Prev_Laser_Rect.right, Prev_Laser_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
bool ALaser_Beam::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Set_At(double x_pos, double y_pos)
{
	X_Pos = x_pos;
	Y_Pos = y_pos;

	Speed = 10.0;
	Laser_Beam_State = ELaser_Beam_State::Active;

	Redraw_Beam();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Redraw_Beam()
{
	Prev_Laser_Rect = Laser_Rect;

	Laser_Rect.left = (int)( (X_Pos - Width / 2.0) * AsConfig::D_Global_Scale);
	Laser_Rect.top = (int) (Y_Pos * AsConfig::D_Global_Scale - 1.0);
	Laser_Rect.right = Laser_Rect.left + Width * AsConfig::Global_Scale + 1;
	Laser_Rect.bottom = Laser_Rect.top + (Height + 1) * AsConfig::Global_Scale - 1;

	AsConfig::Invalidate_Rect(Laser_Rect);
	AsConfig::Invalidate_Rect(Prev_Laser_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool ALaser_Beam::Is_Active()
{
	if (Laser_Beam_State == ELaser_Beam_State::Active)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------