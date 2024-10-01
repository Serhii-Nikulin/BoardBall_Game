#include "Platform_Laser.h"
#include "Laser_Beam_Set.h"

//AsPlatform_Laser
//------------------------------------------------------------------------------------------------------------
AsPlatform_Laser::~AsPlatform_Laser()
{
	Inner_Color = 0;
	Circle_Color = 0;
	White_Color = 0;

	delete Gun_Color;
	Gun_Color = 0;
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_Laser::AsPlatform_Laser(AsPlatform_State &platform_state)
	: Laser_Transformation_Step(0), Inner_Color(0), Circle_Color(0), White_Color(0), Gun_Color(0), Laser_Beam_Set(0), Enable_Laser_Firing(false), Last_Laser_Shot_Tick(0)
{
	Platform_State = &platform_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Init(AsLaser_Beam_Set *laser_beam_set, AColor &inner_color, AColor &circle_color, AColor &white_color)
{
	Laser_Beam_Set = laser_beam_set;

	Inner_Color = &inner_color;
	Circle_Color = &circle_color;
	White_Color = &white_color;

	Gun_Color = new AColor(white_color, AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Laser::Act(EPlatform_State &next_state, double x_pos)
{
	double left_gun_x_pos;
	double right_gun_x_pos;
	double correction;

	next_state = EPlatform_State::Unknown;

	switch (Platform_State->Laser)
	{
	case EPlatform_Transformation::Init:
		if (Laser_Transformation_Step < Max_Laser_Transformation_Step)
			++Laser_Transformation_Step;
		else
			Platform_State->Laser = EPlatform_Transformation::Active;

		return true;

	case EPlatform_Transformation::Active:
		if (Enable_Laser_Firing)
		{
			if (Last_Laser_Shot_Tick + Laser_Shot_Timeout < AsConfig::Current_Timer_Tick)
			{
				correction = 1.0 / AsConfig::D_Global_Scale;
				left_gun_x_pos = Get_Gun_X_Pos(true, x_pos) + correction;
				right_gun_x_pos = Get_Gun_X_Pos(false, x_pos) + correction;

				Laser_Beam_Set->Fire(left_gun_x_pos, right_gun_x_pos);
				Last_Laser_Shot_Tick = AsConfig::Current_Timer_Tick;
			}
		}

		break;

	case EPlatform_Transformation::Finalize:
		if (Laser_Transformation_Step > 0)
			--Laser_Transformation_Step;
		else
		{
			Platform_State->Laser = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
			Enable_Laser_Firing = false;
		}

		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_State(HDC hdc, double x_pos)
{
	Draw_Laser_Inner_Part(hdc, x_pos);

	Draw_Laser_Wing(hdc, true, x_pos);
	Draw_Laser_Wing(hdc, false, x_pos);

	Draw_Laser_Leg(hdc, true, x_pos);
	Draw_Laser_Leg(hdc, false, x_pos);

	Draw_Laser_Cabin(hdc, x_pos);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_Laser_Inner_Part(HDC hdc, double x)
{
	double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;
	int y = AsConfig::Platform_Y_Pos;

	Inner_Color->Select(hdc);
	Draw_Expanding_Figure(hdc, EFigure_Type::Round_Rectangle, x + 4.0, y + 1.0, 20.0, 5.0, ratio, x + 10.0, y + 4.0, 8.0, 1.0);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_Laser_Wing(HDC hdc, bool is_left, double x_pos)
{
	double d_scale = AsConfig::D_Global_Scale;
	int half_max_step = int(Max_Laser_Transformation_Step / 2.0);
	double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;
	double x, y;
	int height;

	AsConfig::BG_Color.Select(hdc);
	Circle_Color->Select(hdc);

	x = x_pos;
	y = AsConfig::Platform_Y_Pos;

	if (! is_left)
		x += AsConfig::Platform_Normal_Width - AsConfig::Platform_Circle_Size;

	Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x, y, AsConfig::Platform_Circle_Size, AsConfig::Platform_Circle_Size, ratio, x, y + 1.0, AsConfig::Platform_Circle_Size, 12);

	//(3, 6) -> (5, 2) - position
	//(1, 1) -> (6, 5) - size

	if (is_left)
		Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 3.0, y + 6.0, 1.0, 1.0, ratio, x + 5.0, y + 2.0, 6.0, 5.0);
	else
		Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 3.0, y + 6.0, 1.0, 1.0, ratio, x - 4.0, y + 2.0, 6.0, 5.0);

	if (Laser_Transformation_Step > half_max_step)
	{
		//gun
		ratio = double(Laser_Transformation_Step - half_max_step) / (double)half_max_step;

		Gun_Color->Select(hdc);

		x = Get_Gun_X_Pos(is_left, x_pos);

		Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x - 1.0, y + 5.0, 0.0, 0.0, ratio, x - 1.0, y + 5.0, 3.0, 6.0);

		height = int(3.0 * d_scale * (1.0 - ratio) );
		MoveToEx(hdc, int(x * d_scale + 1.0), int(y * d_scale + 3.0 * d_scale + 1.0), FALSE);
		LineTo(hdc, int(x * d_scale + 1.0), int(y * d_scale + 1.0 + height) );
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_Laser_Leg(HDC hdc, bool is_left, double x_pos)
{
	int scale = AsConfig::Global_Scale;
	double d_scale = AsConfig::D_Global_Scale;
	double x_scale;
	double x, y;
	double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;

	x_scale = scale;
	y = (AsConfig::Platform_Y_Pos + 3.0) * d_scale;

	if (is_left)
		x = (x_pos + 6.0) * d_scale;
	else
	{
		x = (x_pos + AsConfig::Platform_Normal_Width - 6.0) * d_scale - 1.0;
		x_scale *= -1.0;
	}

	AsConfig::BG_Color.Select(hdc);
	Inner_Color->Select(hdc);

	POINT leg_points[7] = 
	{
		{(int)x, (int)y}, 
		{int(x + 2.0 * x_scale), int(y - 2.0 * d_scale)}, 
		{int(x + 4.0 * x_scale), int(y - 2.0 * d_scale)}, 
		{int(x + 4.0 * x_scale), (int)y}, 
		{int(x + 2.0 * x_scale), int(y + 2.0 * d_scale)}, 
		{int(x + 2.0 * x_scale), int(y + (2.0 + 2.0 * ratio) * d_scale)},
		{(int)x, int(y + 4.0 * d_scale * ratio)},
	};

	Polygon(hdc, leg_points, 7);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_Laser_Cabin(HDC hdc, double x_pos)
{
	double d_scale = AsConfig::D_Global_Scale;
	double x, y;
	int half_max_step = (int)(Max_Laser_Transformation_Step / 2.0);
	double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;

	x = x_pos;
	y = (double)AsConfig::Platform_Y_Pos;

	//red
	Inner_Color->Select(hdc);
	Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13.0, y + 1.0, 2.0, 1.0, ratio, x + 9.0, y - 1.0, 10.0, 8.0);

	//black
	if (Laser_Transformation_Step >= half_max_step)
	{
		ratio = double(Laser_Transformation_Step - half_max_step) / half_max_step;
		AsConfig::BG_Color.Select(hdc);
		Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 10.0, y - 1.0, 8.0, 2.0, ratio, x + 10.0, y - 1.0, 8.0, 6.0);
	}

	//white
	AsConfig::White_Color.Select(hdc);
	ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;

	Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 14.0, y + 2.0, 0.0, 0.0, ratio, x + 11.0, y, 6.0, 4.0);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height)
{
	double x, y;
	double width, height;
	RECT rect{};

	x = Get_Expanding_Value(start_x, end_x, ratio);
	y = Get_Expanding_Value(start_y, end_y, ratio);
	width = Get_Expanding_Value(start_width, end_width, ratio);
	height = Get_Expanding_Value(start_height, end_height, ratio);

	switch (figure_type)
	{
	case EFigure_Type::Rectangle:
		Rectangle(hdc, (int)x, (int)y, int(x + width - 1.0), int(y + height - 1.0) );
		break;

	case EFigure_Type::Ellipse:
		Ellipse(hdc, int(x), int(y), int(x + width - 1.0), int(y + height - 1.0) );
		break;

	case EFigure_Type::Round_Rectangle:
		rect.left = (int)x;
		rect.top = (int)y;
		rect.right = int(x + width - 1.0);
		rect.bottom = int(y + height - 1.0);
		AsConfig::Round_Rect(hdc, rect, AsConfig::Global_Scale);
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform_Laser::Get_Expanding_Value(double start, double end, double ratio)
{
	double value;
	double delta;

	delta = end - start;
	value = (start + delta * ratio) * AsConfig::D_Global_Scale;

	return value;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Reset()
{
	Laser_Transformation_Step = 0;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Laser::Fire(bool fire_on)
{
	if (Platform_State->Laser != EPlatform_Transformation::Active)
		return;

	Enable_Laser_Firing = fire_on;
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform_Laser::Get_Gun_X_Pos(bool is_left, double platform_x_pos)
{
	double gun_x_pos;

	if (is_left)
		gun_x_pos = platform_x_pos + 3.0;
	else
		gun_x_pos = platform_x_pos + AsConfig::Platform_Normal_Width - 4.0;

	return gun_x_pos;
}
//------------------------------------------------------------------------------------------------------------