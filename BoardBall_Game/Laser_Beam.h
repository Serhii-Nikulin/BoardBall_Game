
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class ELaser_Beam_State: unsigned char
{
	Disabled,
	Active,
	Stopping,
	Cleanup
};
//------------------------------------------------------------------------------------------------------------
class ALaser_Beam: public AGame_Object
{
public:
	ALaser_Beam();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Set_At(double x_pos, double y_pos);
	void Redraw_Beam();
	bool Is_Active();

	static AHit_Checker_List Hit_Checker_List;
private:
	void Disable();

	double X_Pos, Y_Pos;
	double Speed;

	static const int Width = 1;
	static const int Height = 3;

	ELaser_Beam_State Laser_Beam_State;

	RECT Laser_Rect;
	RECT Prev_Laser_Rect;
};
//------------------------------------------------------------------------------------------------------------