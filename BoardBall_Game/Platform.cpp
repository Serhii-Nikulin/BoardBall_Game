#include "Platform.h"

int AsPlatform::Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

const double AsPlatform_Adhesive::Step_Adhesive_Spot_Height_Ratio = 0.04;
const double AsPlatform_Adhesive::Min_Adhesive_Spot_Height_Ratio = 0.0;
const double AsPlatform_Adhesive::Max_Adhesive_Spot_Height_Ratio = 1.0;

const double AsPlatform_Expanding::Step_Expanding_Width = 0.5;
const double AsPlatform_Expanding::Min_Expanding_Width = 28.0;
const double AsPlatform_Expanding::Max_Expanding_Width = 40.0;

//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State() :
	Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing), Meltdown(EPlatform_Substate_Meltdown::Unknown), Rolling(EPlatform_Substate_Rolling::Unknown), Adhesive(EPlatform_Transformation::Unknown), Expanding(EPlatform_Transformation::Unknown), Laser(EPlatform_Transformation::Unknown), Moving_State(EPlatform_Moving_State::Stop)
{
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::operator EPlatform_State() const
{
	return Current_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::operator = (const EPlatform_State &new_platform_state)
{
	Current_State = new_platform_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
	if (next_state == Current_State)
		return;

	switch (Current_State)
	{
	case EPlatform_State::Regular:
		AsConfig::Throw();

	case EPlatform_State::Meltdown:
		break;

	case EPlatform_State::Rolling:
		AsConfig::Throw();

	case EPlatform_State::Adhesive:
		Adhesive = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Expanding:
		Expanding = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Laser:
		Laser = EPlatform_Transformation::Finalize;
		break;

	default:
		AsConfig::Throw();
	}

	Next_State = next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Get_Next_State() const
{
	return Next_State;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_State next_state = Get_Next_State();
	Current_State = EPlatform_State::Regular;

	if (next_state == EPlatform_State::Unknown)
		Regular = new_regular_state;

	return next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_Transformation *transformation_state = 0;
	EPlatform_State next_state;

	if (Current_State == EPlatform_State::Regular and Regular == new_regular_state)
		return EPlatform_State::Unknown;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		switch (Current_State)
		{
		case EPlatform_State::Adhesive:
			transformation_state = &Adhesive;
			break;

		case EPlatform_State::Expanding:
			transformation_state = &Expanding;
			break;

		case EPlatform_State::Laser:
			transformation_state = &Laser;
			break;
		}

		if (transformation_state != 0)
		{
			if (*transformation_state == EPlatform_Transformation::Unknown)
			{
				next_state = Set_Next_Or_Regular_State(new_regular_state);

				return next_state;
			}
			else 
				if(*transformation_state == EPlatform_Transformation::Active or *transformation_state == EPlatform_Transformation::Init)
					*transformation_state =  EPlatform_Transformation::Finalize;

			return EPlatform_State::Unknown;
		}
	}

	Current_State = EPlatform_State::Regular;
	Regular = new_regular_state;

	return EPlatform_State::Unknown;
}
//------------------------------------------------------------------------------------------------------------




//AsPlatform_Adhesive
//------------------------------------------------------------------------------------------------------------
AsPlatform_Adhesive::AsPlatform_Adhesive(AsPlatform_State &platform_state)
: Adhesive_Spot_Height_Ratio(0.0), Platform_State(&platform_state)

{
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Adhesive::Act(AsBall_Set *ball_set, EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;

	switch (Platform_State->Adhesive)
	{
	case EPlatform_Transformation::Init:
		if (Adhesive_Spot_Height_Ratio < Max_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio += Step_Adhesive_Spot_Height_Ratio;
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Max_Adhesive_Spot_Height_Ratio;
			Platform_State->Adhesive = EPlatform_Transformation::Active;
		}

		return true;

	case EPlatform_Transformation::Finalize:
		if (Adhesive_Spot_Height_Ratio > Min_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio -= Step_Adhesive_Spot_Height_Ratio;

			while (ball_set->Release_Next_Ball() )
			{
			}
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
			Platform_State->Adhesive = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
		}

		return true;
	}	

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Draw_State(HDC hdc, double x_pos)
{
	AsConfig::White_Color.Select(hdc);
	AsConfig::BG_Color.Select_Pen(hdc);
	
	Draw_Adhesive_Spot(hdc, x_pos, 0, 7, 4);
	Draw_Adhesive_Spot(hdc, x_pos, 5, 5, 5);
	Draw_Adhesive_Spot(hdc, x_pos, 7, 9, 5);

	AsConfig::White_Color.Select(hdc);

	Draw_Adhesive_Spot(hdc, x_pos, 0, 7, 3);
	Draw_Adhesive_Spot(hdc, x_pos, 5, 5, 4);
	Draw_Adhesive_Spot(hdc, x_pos, 7, 9, 4);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Draw_Adhesive_Spot(HDC hdc, double x_pos, int x_offset, int width, int height)
{
	RECT spot_rect{};

	int x = (int)((x_pos + 6.0) * AsConfig::D_Global_Scale);
	int y = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;

	int spot_height = int( (double)height * AsConfig::D_Global_Scale * Adhesive_Spot_Height_Ratio);

	spot_rect.left = x + x_offset * AsConfig::Global_Scale - 1;
	spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale + 1;
	spot_rect.top = y - spot_height;
	spot_rect.bottom = y + spot_height - 1;

	Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right, spot_rect.bottom, x, y, spot_rect.right, y);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Reset()
{
	Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
}
//------------------------------------------------------------------------------------------------------------




//AsPlatform_Expanding
//------------------------------------------------------------------------------------------------------------
AsPlatform_Expanding::~AsPlatform_Expanding()
{
	Inner_Color = 0;
	Circle_Color = 0;
	Truss_Expanding_Color = 0;

	delete Truss_Expanding_Color;
	Truss_Expanding_Color = 0;
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_Expanding::AsPlatform_Expanding(AsPlatform_State &platform_state)
: Platform_State(&platform_state), Inner_Color(0), Circle_Color(0), Highlight_Color(0), Truss_Expanding_Color(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Expanding::Act(double &x_pos, double &current_width, EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;
	double offset = Step_Expanding_Width / 2.0;

	switch (Platform_State->Expanding)
	{
	case EPlatform_Transformation::Init:
		if (current_width < Max_Expanding_Width)
		{
			current_width += Step_Expanding_Width;

			if (x_pos - offset > AsConfig::Border_X_Offset)
			{
				if (x_pos < AsConfig::Max_X_Pos - current_width + 1.0)
					x_pos -= offset;
				else
					x_pos -= Step_Expanding_Width;
			}
		}
		else
		{
			current_width = Max_Expanding_Width;
			Platform_State->Expanding = EPlatform_Transformation::Active;
		}	

		return true;

	case EPlatform_Transformation::Finalize:
		if (current_width > Min_Expanding_Width)
		{
			current_width -= Step_Expanding_Width;
			x_pos += offset;
		}
		else
		{
			current_width = Min_Expanding_Width;
			Platform_State->Expanding = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
		}
		
		return true;
	}	

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_State(HDC hdc, double x, double &current_width, RECT &Platform_Rect)
{
	double d_scale = AsConfig::D_Global_Scale;
	int scale = AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos;
	int ellipse_offset = 4;
	int ellipse_size = 5;
	double middle_pos;
	double ratio;
	int x_offset;
	RECT inner_rect{}, ellipse_rect{};

	middle_pos = x + current_width / 2.0;

	inner_rect.left = int( (middle_pos - AsPlatform::Expanding_Platform_Inner_Width / 2.0) * d_scale);
	inner_rect.right = inner_rect.left + AsPlatform::Expanding_Platform_Inner_Width * scale;
	inner_rect.top = (y + 1) * scale;
	inner_rect.bottom = inner_rect.top + AsPlatform::Inner_Height * scale - 1;

	//0.Side parts
	Circle_Color->Select(hdc);

	Ellipse(hdc, Platform_Rect.left, Platform_Rect.top, Platform_Rect.left + AsPlatform::Circle_Size * scale - 1, Platform_Rect.bottom - 1);
	Ellipse(hdc, Platform_Rect.right - AsPlatform::Circle_Size * scale, Platform_Rect.top, Platform_Rect.right - 1, Platform_Rect.bottom - 1);

	Draw_Circle_Highlight(hdc, int(x * d_scale), y * scale);

	//1.Left part
	ellipse_rect.top = (y + 1) * scale;
	ellipse_rect.bottom = ellipse_rect.top + ellipse_size * scale - 1;
	ellipse_rect.left = Platform_Rect.left + ellipse_offset * scale;
	ellipse_rect.right = ellipse_rect.left + ellipse_size * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Ellipse(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom);
	Truss_Expanding_Color->Select_Pen(hdc);
	Arc(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom,
		ellipse_rect.left, ellipse_rect.top, ellipse_rect.left, ellipse_rect.bottom);

	//2.Right part
	ellipse_rect.left = Platform_Rect.right - (ellipse_offset + ellipse_size) * scale;
	ellipse_rect.right = ellipse_rect.left + ellipse_size * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Ellipse(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom);
	Truss_Expanding_Color->Select_Pen(hdc);
	Arc(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom,
		ellipse_rect.right, ellipse_rect.bottom, ellipse_rect.right, ellipse_rect.top);

	//3.Inner part
	Inner_Color->Select(hdc);
	Rectangle(hdc, inner_rect.left, inner_rect.top, inner_rect.right, inner_rect.bottom);

	//4.Truss
	ratio = current_width / Max_Expanding_Width;
	Draw_Expanding_Truss(hdc, x, y, ratio);
	x_offset = int( ( (Max_Expanding_Width - AsPlatform::Expanding_Platform_Inner_Width - ellipse_offset * 2.0) / 2.0 + AsPlatform::Expanding_Platform_Inner_Width) * ratio - 1);
	x += x_offset;
	Draw_Expanding_Truss(hdc, x, y, ratio);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
	Highlight_Color->Select_Pen(hdc);

	Arc(hdc,
		x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
		x + (AsPlatform::Circle_Size - 1) * AsConfig::Global_Scale, y + (AsPlatform::Circle_Size - 1) * AsConfig::Global_Scale,
		x + (AsPlatform::Circle_Size / 2) * AsConfig::Global_Scale, y,
		x, y + (AsPlatform::Circle_Size / 2) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_Expanding_Truss(HDC hdc, double x, int y, double ratio)
{
	int scale = AsConfig::Global_Scale;
	double d_scale = AsConfig::D_Global_Scale;

	Truss_Expanding_Color->Select_Pen(hdc);

	MoveToEx(hdc, int( (x + 5) * scale + 1), (y + 2) * scale - 1, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 5) * scale + 1);
	LineTo(hdc, int( (x + 14 * ratio) * d_scale - 1), (y + 2) * scale - 1);

	MoveToEx(hdc, int( (x + 5) * d_scale + 1), (y + 5) * scale, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 1) * scale + 1);
	LineTo(hdc, int( (x + 14 * ratio) * d_scale - 1), (y + 5) * scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Reset(double &width)
{
	width = Min_Expanding_Width;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Init(AColor &inner_color, AColor &circle_color, AColor &highlight_color)
{
	Inner_Color = &inner_color;
	Circle_Color = &circle_color;
	Highlight_Color = &highlight_color;

	Truss_Expanding_Color = new AColor(*Inner_Color, AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------




//ALaser_Beam
ALaser_Beam::ALaser_Beam()
	: Is_Active(false), X_Pos(0), Y_Pos(0), Speed(0.0), Laser_Rect{}, Prev_Laser_Rect{}
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
	if (Is_Active)
		Redraw_Beam();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Shift_Per_Step(double max_speed)
{
	double next_step;

	if (! Is_Active)
		return;

	next_step = AsConfig::Moving_Step_Size * Speed / max_speed;

	Y_Pos -= next_step;

	if (Y_Pos < AsConfig::Level_Y_Offset)
		Is_Active = false;
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

	if (! Is_Active)
		return;

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

	if (! Is_Active)
		return;

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
	Is_Active = true;

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
		if (Laser_Beams[i].Is_Active)
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
: Laser_Transformation_Step(0), Inner_Color(0), Circle_Color(0), White_Color(0), Gun_Color(0), Laser_Beam_Set(0)
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
bool AsPlatform_Laser::Act(EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;

	switch (Platform_State->Laser)
	{
	case EPlatform_Transformation::Init:
		if (Laser_Transformation_Step < Max_Laser_Transformation_Step)
			++Laser_Transformation_Step;
		else
			Platform_State->Laser = EPlatform_Transformation::Active;

		return true;

	case EPlatform_Transformation::Finalize:
		if (Laser_Transformation_Step > 0)
			--Laser_Transformation_Step;
		else
		{
			Platform_State->Laser = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
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
		x += AsPlatform::Normal_Width - AsPlatform::Circle_Size;

	Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x, y, AsPlatform::Circle_Size, AsPlatform::Circle_Size, ratio, x, y + 1.0, AsPlatform::Circle_Size, 12);

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
		x = (x_pos + AsPlatform::Normal_Width - 6.0) * d_scale - 1.0;
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
void AsPlatform_Laser::Fire(bool fire_on, double x_pos)
{
	double left_gun_x_pos;
	double right_gun_x_pos;
	double correction;

	if (Platform_State->Laser != EPlatform_Transformation::Active)
		return;

	if (! fire_on)
		return;

	correction = 1.0 / AsConfig::D_Global_Scale;

	left_gun_x_pos = Get_Gun_X_Pos(true, x_pos) + correction;
	right_gun_x_pos = Get_Gun_X_Pos(false, x_pos) + correction;

	Laser_Beam_Set->Fire(left_gun_x_pos, right_gun_x_pos);
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform_Laser::Get_Gun_X_Pos(bool is_left, double platform_x_pos)
{
	double gun_x_pos;

	if (is_left)
		gun_x_pos = platform_x_pos + 3.0;
	else
		gun_x_pos = platform_x_pos + AsPlatform::Normal_Width - 4.0;

	return gun_x_pos;
}
//------------------------------------------------------------------------------------------------------------




//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform() :
	Inner_Width(Normal_Inner_Width), Rolling_Step(0), Width(28), X_Pos(AsConfig::Start_Ball_Position_On_Platform - Width / 2), Prev_Platform_Rect{}, Platform_Rect{}, Normal_Platform_Image_Width (28 * AsConfig::Global_Scale), Ball_Set(0), Normal_Platform_Image_Height(Height * AsConfig::Global_Scale),Normal_Platform_Image(0), Platform_Inner_Color(237, 38, 36), Platform_Circle_Color(63, 72, 204), Highlight_Color(255, 255, 255), Truss_Expanding_Color(Platform_Inner_Color, AsConfig::Global_Scale), Gun_Color(Highlight_Color, AsConfig::Global_Scale), Speed(0.0), Last_Redraw_Timer_Tick(0), Left_Key_Down(false), Right_Key_Down(false), Platform_Adhesive(Platform_State), Platform_Expanding(Platform_State), Platform_Laser(Platform_State)
{}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(AsLaser_Beam_Set *laser_beam_set, AsBall_Set *ball_set)
{
	Ball_Set = ball_set;
	Platform_Expanding.Init(Platform_Inner_Color, Platform_Circle_Color, Highlight_Color);
	Platform_Laser.Init(laser_beam_set, Platform_Inner_Color, Platform_Circle_Color, Highlight_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	double inner_top_y;
	double inner_low_y;
	double inner_left_x;
	double inner_right_x;

	double x_pos, y_pos;
	double speed;

	if (next_y_pos < AsConfig::Platform_Y_Pos)
		return false;

	if (Reflect_On_Circle(next_x_pos, next_y_pos, ball))//from left
		goto got_hit;

	if (Reflect_On_Circle(next_x_pos, next_y_pos, ball, + (Width - Circle_Size)))//from right
		goto got_hit;

	inner_top_y = AsConfig::Platform_Y_Pos + 1;
	inner_low_y = AsConfig::Platform_Y_Pos + Inner_Height + 1;
	inner_left_x = double(X_Pos + Circle_Size - 1 + ball->Radius);
	inner_right_x = double(X_Pos + Width - Circle_Size - ball->Radius);

	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - inner_low_y, ball->Radius, inner_left_x, inner_right_x))
		{
			ball->Reflect(true);//from platform horizontal
			goto got_hit;
		}
	}
	else
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - inner_top_y, ball->Radius, inner_left_x, inner_right_x))
		{
			ball->Reflect(true);//from platform horizontal
			goto got_hit;
		}
	}

	return false;

got_hit:

	if (ball->Get_State() == EBS_On_Parachute)
		ball->Set_State(EBS_Off_Parachute);

	if (Platform_State == EPlatform_State::Adhesive and Platform_State.Adhesive == EPlatform_Transformation::Active)
	{
		speed = ball->Get_Speed();
		ball->Get_Center(x_pos, y_pos);
		ball->Set_State(EBS_On_Platform, x_pos, y_pos, ball->Get_Direction() );
		ball->Set_Speed(speed);
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Begin_Movement()
{
	if (Platform_State != EPlatform_State::Expanding)
	Prev_Platform_Rect = Platform_Rect;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Finish_Movement()
{
	if (Platform_State.Moving_State == EPlatform_Moving_State::Stop)
		return;

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform::Get_Speed()
{
	return Speed;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
	EPlatform_State next_state;

	switch (Platform_State)
	{
	case EPlatform_State::Meltdown:
		Act_For_Meltdown_State();
		break;

	case EPlatform_State::Rolling:
		Act_For_Rolling_State();
		break;

	case EPlatform_State::Adhesive:
		if (Platform_Adhesive.Act(Ball_Set, next_state) )
			Redraw();

		if (next_state != EPlatform_State::Unknown)
			Set_State(next_state);

		break;

	case EPlatform_State::Expanding:
		if (Platform_Expanding.Act(X_Pos, Width, next_state) )
			Redraw();

		if (next_state != EPlatform_State::Unknown)
			Set_State(next_state);

		break;

	case EPlatform_State::Laser:
		if (Platform_Laser.Act(next_state) )
			Redraw();

		if (next_state != EPlatform_State::Unknown)
			Set_State(next_state);

		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act_For_Meltdown_State()
{
	switch (Platform_State.Meltdown)
	{
	case EPlatform_Substate_Meltdown::Init:
		Platform_State.Meltdown = EPlatform_Substate_Meltdown::Active;
		break;

	case EPlatform_Substate_Meltdown::Active:
		Redraw();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act_For_Rolling_State()
{
	switch (Platform_State.Rolling)
	{
	case EPlatform_Substate_Rolling::Roll_In:
		Rolling_Step += 1;
		Rolling_Step %= Max_Rolling_Step;
		X_Pos -= Rolling_Platform_Speed;

		if (X_Pos <= Roll_In_Platform_End_X_Pos)
		{
			X_Pos = Roll_In_Platform_End_X_Pos - 1.0 / AsConfig::D_Global_Scale;
			Platform_State.Rolling = EPlatform_Substate_Rolling::Expand_Roll_In;
		}
		break;

	case EPlatform_Substate_Rolling::Expand_Roll_In:
		if (Inner_Width < Normal_Inner_Width)
		{
			Inner_Width += 2;
			X_Pos -= 1;
			Platform_Rect.left = (int)X_Pos;
		}
		else
		{
			Inner_Width = Normal_Inner_Width;
			Set_State(EPlatform_Substate_Regular::Ready);
			Platform_State.Rolling = EPlatform_Substate_Rolling::Unknown;
		}
		break;
	}

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	switch (Platform_State)
	{
	case EPlatform_State::Regular:
		if (Platform_State.Regular == EPlatform_Substate_Regular::Ready or Platform_State.Regular == EPlatform_Substate_Regular::Normal)
			Draw_Normal_State(hdc, paint_area);
		break;

	case EPlatform_State::Meltdown:
		if (Platform_State.Meltdown == EPlatform_Substate_Meltdown::Active)
			Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPlatform_State::Rolling:
		Draw_Rolling_State(hdc, paint_area);
		break;

	case EPlatform_State::Adhesive:
		Draw_Normal_State(hdc, paint_area);
		Platform_Adhesive.Draw_State(hdc, X_Pos);
		break;

	case EPlatform_State::Expanding:
		if (IntersectRect(&intersection_rect, &Platform_Rect, &paint_area) )
			Platform_Expanding.Draw_State(hdc, X_Pos, Width, Platform_Rect);

		break;

	case EPlatform_State::Laser:
		HRGN region = CreateRectRgnIndirect(&Platform_Rect);
		SelectClipRgn(hdc, region);

		Platform_Laser.Draw_State(hdc, X_Pos);

		SelectClipRgn(hdc, 0);
		DeleteObject(region);

		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Rolling_State(HDC hdc, RECT &paint_area)
{
	switch (Platform_State.Rolling)
	{
	case EPlatform_Substate_Rolling::Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;

	case EPlatform_Substate_Rolling::Expand_Roll_In:
		Draw_Normal_State(hdc, paint_area);
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect) )
		return;

	switch (Platform_State)
	{
	case EPlatform_State::Meltdown:
	case EPlatform_State::Regular:
		if (Platform_State.Regular == EPlatform_Substate_Regular::Missing)
			break;

	case EPlatform_State::Rolling:
	case EPlatform_State::Adhesive:
	case EPlatform_State::Expanding:
	case EPlatform_State::Laser:
		AsConfig::BG_Color.Select(hdc);
		Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
	default:
		return;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Shift_Per_Step(double max_speed)
{
	double step = AsConfig::Moving_Step_Size * Get_Speed() / max_speed;

	X_Pos += step;

	if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Left)
	{
		if (X_Pos < AsConfig::Border_X_Offset)
		{
			X_Pos = AsConfig::Border_X_Offset;
			Speed = 0.0;
		}
	}
	else if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Right)
	{
		if (X_Pos > AsConfig::Max_X_Pos - Width + 1)
		{
			X_Pos = AsConfig::Max_X_Pos - Width + 1;
			Speed = 0.0;
		}
	}

	if (Platform_State == EPlatform_State::Adhesive and Platform_State.Adhesive == EPlatform_Transformation::Active and Speed != 0)
	{
		if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Left)
		{
			Ball_Set->Shift_By_Platform(M_PI, fabs(Speed), max_speed);
		}
		else if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Right)
		{
			Ball_Set->Shift_By_Platform(0.0, fabs(Speed), max_speed);
		}
	}		
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, ABall *ball, double x_offset)
{
	double angle_to_normal;
	double reflect_angle;
	double full_reflect_angle;

	double circle_radius = Circle_Size / 2.0;
	double platform_ball_x, platform_ball_y;
	double dx, dy;
	double direction;
	double speed;

	double ball_left_x = next_x_pos - ball->Radius;
	double ball_right_x = next_x_pos + ball->Radius;
	double ball_top_y = next_y_pos - ball->Radius;
	double ball_low_y = next_y_pos + ball->Radius;

	platform_ball_x = X_Pos + x_offset + circle_radius;
	platform_ball_y = AsConfig::Platform_Y_Pos + circle_radius;
	dx = next_x_pos - platform_ball_x;
	dy = platform_ball_y - next_y_pos;

	if ( (ball_right_x > X_Pos + x_offset and ball_left_x < X_Pos + Circle_Size - 1 + x_offset) 
		and 
		(ball_low_y > AsConfig::Platform_Y_Pos and ball_top_y < AsConfig::Platform_Y_Pos + Circle_Size - 1) )
	{
		angle_to_normal = atan2(dy, dx);

		if (fabs(ball->prev_angle_to_normal - angle_to_normal) <= 1E-5)
		{
			direction = ball->Get_Direction() + M_PI;
		}
		else
		{
			reflect_angle = angle_to_normal + M_PI - ball->Get_Direction();
			full_reflect_angle = angle_to_normal + reflect_angle;
			direction = full_reflect_angle;
			//ball->Set_Direction(full_reflect_angle);
		}

		if (ball->Get_State() == EBS_On_Parachute)
			ball->Set_State(EBS_Off_Parachute);
		else
		{
			speed = ball->Get_Speed();
			ball->Set_State(EBS_Normal, (int)(next_x_pos + AsConfig::Global_Scale * cos(direction)), (int)(next_y_pos - AsConfig::Global_Scale * sin(direction) ), direction);
			ball->Set_Speed(speed);
		}

		ball->prev_angle_to_normal = angle_to_normal;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
	int i, len;

	//if (new_state == Platform_State and Platform_State == EPlatform_State::Adhesive)
	//	if (Platform_State.Adhesive == EPlatform_Substate_Adhesive::Finalize)
	//		Platform_State.Adhesive = EPlatform_Substate_Adhesive::Init;

	if (Platform_State == new_state)
		return;

	switch (new_state)
	{
	case EPlatform_State::Regular:
		AsConfig::Throw();		

	case EPlatform_State::Rolling:
		X_Pos = AsConfig::Max_X_Pos;
		Rolling_Step = Max_Rolling_Step - 1;
		Inner_Width = 0;
		Platform_State.Rolling = EPlatform_Substate_Rolling::Roll_In;

		break;

	case EPlatform_State::Meltdown:
		if (Platform_State != EPlatform_State::Regular)
		{
			Platform_State.Set_Next_State(new_state);
			return;
		}

		Speed = 0.0;
		Platform_State.Meltdown = EPlatform_Substate_Meltdown::Init;

		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(int);
		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;

		break;

	case EPlatform_State::Adhesive:

		if (Set_Transformation_State(new_state, Platform_State.Adhesive) )
			return;
		else
			Platform_Adhesive.Reset();

		break;

	case EPlatform_State::Expanding:

		if (Set_Transformation_State(new_state, Platform_State.Expanding) )
			return;
		else
			Platform_Expanding.Reset(Width);

		break;

	case EPlatform_State::Laser:

		if (Set_Transformation_State(new_state, Platform_State.Laser) )
			return;
		else
			Platform_Laser.Reset();
			
		break;
	}

	Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_State next_state = Platform_State.Set_State(new_regular_state);

	if (next_state != EPlatform_State::Unknown)
		Set_State(next_state);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation &transformation_state)
{
	if (Platform_State != EPlatform_State::Regular)
	{
		Platform_State.Set_Next_State(new_state);
		return true;
	}

	if (transformation_state == EPlatform_Transformation::Finalize)
		return true;
	else
	{
		transformation_state = EPlatform_Transformation::Init;
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State()
{
	return Platform_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{
	double x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;
	double d_scale = AsConfig::D_Global_Scale;
	int scale = AsConfig::Global_Scale;
	RECT intersection_rect{};
	RECT inner_rect{};

	//draw side parts
	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, int (x * d_scale), y * scale, int ( (x + Circle_Size) * d_scale - 1), (y + Circle_Size) * scale - 1);
	Ellipse(hdc, int( (x + Inner_Width + 1) * d_scale), y * scale, int( (x + Inner_Width + Circle_Size + 1) * d_scale - 1), (y + Circle_Size) * scale - 1);

	//draw inner part
	Platform_Inner_Color.Select(hdc);

	inner_rect.left = int( (x + 4) * d_scale);
	inner_rect.top = (y + 1) * scale;
	inner_rect.right = int( (x + 4 + Inner_Width) * d_scale);
	inner_rect.bottom = (y + 1 + Inner_Height) * scale;

	AsConfig::Round_Rect(hdc, inner_rect, Inner_Height);

	//draw highlight
	Platform_Expanding.Draw_Circle_Highlight(hdc, int(x * d_scale), y * scale);

	if (Normal_Platform_Image == 0 and Has_State(EPlatform_Substate_Regular::Ready) )
		Get_Normal_Platform_Image(hdc);	
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Get_Normal_Platform_Image(HDC hdc)
{
	int i, j;
	int x = (int)X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int offset = 0;

	Normal_Platform_Image = new int[Normal_Platform_Image_Height * Normal_Platform_Image_Width];

	for (i = 0; i < Normal_Platform_Image_Height; i++)
		for (j = 0; j < Normal_Platform_Image_Width; j++)
			Normal_Platform_Image[offset++] = GetPixel(hdc, (int)x + j, y + i);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{
	int i, j;
	int y_offset;
	int x, y;
	int moved_columns_count = 0;
	int max_platform_y;
	
	int stroke_len;
	const AColor *color;

	max_platform_y = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

	for (i = 0; i < Normal_Platform_Image_Width; i++)
	{
		if (Meltdown_Platform_Y_Pos[i] > max_platform_y)
			continue;

		y_offset = AsConfig::Rand(Meltdown_Speed);

		x = Platform_Rect.left + i;
		y = Meltdown_Platform_Y_Pos[i];
		MoveToEx(hdc, x, y, 0);
		j = 0;

		while (Get_Platform_Image_Storke_Color(i, j, stroke_len, &color) )
		{
			y += stroke_len;
			j += stroke_len;
			color->Select_Pen(hdc);
			LineTo(hdc, x, y);			
		}

		y = Meltdown_Platform_Y_Pos[i];
		MoveToEx(hdc, x, y, 0);
		AsConfig::BG_Color.Select_Pen(hdc);
		LineTo(hdc, x, y + y_offset);
		Meltdown_Platform_Y_Pos[i] += y_offset;

		moved_columns_count += 1;
	}

	if (moved_columns_count == 0)
		Set_State(EPlatform_Substate_Regular::Missing);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Get_Platform_Image_Storke_Color(int x, int y, int &stroke_len, const AColor **color)
{
	int i;
	int offset = y * Normal_Platform_Image_Width + x;
	COLORREF color_value;

	if (y >= Normal_Platform_Image_Height)
		return false;

	for (i = y; i < Normal_Platform_Image_Height; i++)
	{
		if (i == y)
		{
			stroke_len = 1;
			color_value = Normal_Platform_Image[offset];
		}
		else
		{
			if (Normal_Platform_Image[offset] == color_value)
				stroke_len += 1;
			else
				break;
		}

		offset += Normal_Platform_Image_Width;
	}

	if (color_value == AsConfig::BG_Color.Get_RGB())
		*color = &AsConfig::BG_Color;
	else if (color_value == Platform_Inner_Color.Get_RGB())
		*color = &Platform_Inner_Color;
	else if (color_value == Platform_Circle_Color.Get_RGB())
		*color = &Platform_Circle_Color;
	else if (color_value = Highlight_Color.Get_RGB())	
		*color = &Highlight_Color;
	else
		throw 13;

	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw(bool update_rect)
{
	int platform_width;

	if (Last_Redraw_Timer_Tick != AsConfig::Current_Timer_Tick)
	{
		Prev_Platform_Rect = Platform_Rect;
		Last_Redraw_Timer_Tick = AsConfig::Current_Timer_Tick;
	}
	
	if (Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
		platform_width = Circle_Size;
	else
		platform_width = int(Width);

	Platform_Rect.left = int(X_Pos * AsConfig::D_Global_Scale);
	Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	Platform_Rect.right = int(Platform_Rect.left + platform_width * AsConfig::D_Global_Scale);
	Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

	if (Platform_State == EPlatform_State::Meltdown)
	{
		Prev_Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + AsConfig::Global_Scale;
	}
	
	AsConfig::Invalidate_Rect(Prev_Platform_Rect);
	AsConfig::Invalidate_Rect(Platform_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect{};
	double x = (int)X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int roller_size = Circle_Size * AsConfig::Global_Scale;
	XFORM xform{}, prev_xform{};
	double rotation_angle;

	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, (int)x, y, (int)x + roller_size - 1, y + roller_size);

	GetWorldTransform(hdc, &prev_xform);

	rotation_angle = 2.0 * M_PI * Rolling_Step / (double)Max_Rolling_Step;

	xform.eM11 = (FLOAT)cos(rotation_angle); xform.eM12 = (FLOAT)-sin(rotation_angle);
	xform.eM21 = (FLOAT)sin(rotation_angle); xform.eM22 = (FLOAT)cos(rotation_angle);
	xform.eDx = (FLOAT)(x + roller_size / 2.0);
	xform.eDy = (FLOAT)(y + roller_size / 2.0);

	SetWorldTransform(hdc, &xform);
	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, -1, -roller_size / 2, 1, roller_size / 2);

	SetWorldTransform(hdc, &prev_xform);

	Platform_Expanding.Draw_Circle_Highlight(hdc, (int)x, y);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left, bool key_down)
{
	/*if (Platform_State != EPS_Normal)
		return;*/

	if (! ( Has_State(EPlatform_Substate_Regular::Normal) or Platform_State == EPlatform_State::Adhesive or Platform_State == EPlatform_State::Expanding or Platform_State == EPlatform_State::Laser) ) 
		return;

	if (to_left)
		Left_Key_Down = key_down;
	else
		Right_Key_Down = key_down;

	if (Left_Key_Down and Right_Key_Down)
		return;

	if (!Left_Key_Down and !Right_Key_Down)
	{
		Platform_State.Moving_State = EPlatform_Moving_State::Stop;
		Speed = 0.0;
		return;
	}

	if (Left_Key_Down)
	{
		Platform_State.Moving_State = EPlatform_Moving_State::Moving_Left;	
		Speed = -X_Step;
	}

	if(Right_Key_Down)
	{
		Platform_State.Moving_State = EPlatform_Moving_State::Moving_Right;	
		Speed = X_Step;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Has_State(EPlatform_Substate_Regular regular_state)
{
	if (Platform_State != EPlatform_State::Regular)
		return false;

	if (Platform_State.Regular == regular_state)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Hit_By(AFalling_Letter *falling_letter) const
{
	RECT intersection_rect, falling_letter_rect;
	falling_letter->Get_Letter_Cell(falling_letter_rect);

	if (IntersectRect(&intersection_rect, &falling_letter_rect, &Platform_Rect) )
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::On_Space_Key(bool key_down)
{
	if (! key_down)
		return;

	if (Has_State(EPlatform_Substate_Regular::Ready) )
	{
		Ball_Set->Release_From_Platform();
		Set_State(EPlatform_Substate_Regular::Normal);
	}
	else
		if (Platform_State == EPlatform_State::Adhesive)
			Ball_Set->Release_Next_Ball();
		else if (Platform_State == EPlatform_State::Laser)
			Platform_Laser.Fire(key_down, X_Pos);
}
//------------------------------------------------------------------------------------------------------------