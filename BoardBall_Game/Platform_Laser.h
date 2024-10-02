#include "Platform_State.h"
#include "Laser_Beam_Set.h"

//------------------------------------------------------------------------------------------------------------
class AsPlatform_Laser
{
public:
	~AsPlatform_Laser();
	AsPlatform_Laser(AsPlatform_State &platform_state);
	void Init(AsLaser_Beam_Set *laser_beam_set, AColor &inner_color, AColor &circle_color, AColor &white_color);
	bool Act(EPlatform_State &next_state, double x_pos);
	void Draw_State(HDC hdc, double x_pos);
	void Reset();
	void Fire(bool fire_on);
private:
	void Draw_Laser_Inner_Part(HDC hdc, double x_pos);
	void Draw_Laser_Wing(HDC hdc, bool is_left, double x_pos);
	void Draw_Laser_Leg(HDC hdc, bool is_left, double x_pos);
	void Draw_Laser_Cabin(HDC hdc, double x_pos);
	void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
	double Get_Expanding_Value(double start, double end, double ratio);
	double Get_Gun_X_Pos(bool is_left, double platform_x_pos);

	AsPlatform_State *Platform_State;

	AColor *Inner_Color, *Circle_Color, *White_Color;
	AColor *Gun_Color;

	AsLaser_Beam_Set *Laser_Beam_Set;// UNO

	bool Enable_Laser_Firing;
	int Laser_Transformation_Step;

	int Last_Laser_Shot_Tick;

	static const int Laser_Shot_Timeout = AsConfig::FPS / 2;
	static const int Max_Laser_Transformation_Step = 30;
};
//------------------------------------------------------------------------------------------------------------