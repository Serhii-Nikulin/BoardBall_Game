#include "Platform.h"

int AsPlatform::Meltdown_Platform_Y_Pos[Normal_Width];
const double AsPlatform::Step_Adhesive_Spot_Height_Ratio = 0.04;
const double AsPlatform::Min_Adhesive_Spot_Height_Ratio = 0.0;
const double AsPlatform::Max_Adhesive_Spot_Height_Ratio = 1.0;

const double AsPlatform::Step_Expanding_Width = 0.5;
const double AsPlatform::Min_Expanding_Width = 28.0;
const double AsPlatform::Max_Expanding_Width = 40.0;

//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State() :
	Current_State(EPlatform_State::Regular), Regular(EPlatform_Substate_Regular::Missing), Meltdown(EPlatform_Substate_Meltdown::Unknown), Rolling(EPlatform_Substate_Rolling::Unknown), Adhesive(EPlatform_Substate_Adhesive::Unknown), Expanding(EPlatform_Substate_Expanding::Unknown), Moving_State(EPlatform_Moving_State::Stop)
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
AsPlatform::AsPlatform() :
	Inner_Width(Normal_Inner_Width), Rolling_Step(0), Width(28), X_Pos(AsConfig::Start_Ball_Position_On_Platform - Width / 2), Prev_Platform_Rect{}, Platform_Rect{}, Normal_Platform_Image_Width (28 * AsConfig::Global_Scale), Ball_Set(0), Normal_Platform_Image_Height(Height * AsConfig::Global_Scale),Normal_Platform_Image(0), Platform_Inner_Color(237, 38, 36), Platform_Circle_Color(63, 72, 204), Highlight_Color(255, 255, 255), Truss_Expanding_Color(Platform_Inner_Color, AsConfig::Global_Scale), Speed(0.0), Left_Key_Down(false), Right_Key_Down(false), Adhesive_Spot_Height_Ratio(0.0)
{}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(AsBall_Set *ball_set)
{
	Ball_Set = ball_set;
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

	if (Platform_State == EPlatform_State::Adhesive and Platform_State.Adhesive == EPlatform_Substate_Adhesive::Active)
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
	switch (Platform_State)
	{
	case EPlatform_State::Meltdown:
		Act_For_Meltdown_State();
		break;

	case EPlatform_State::Rolling:
		Act_For_Rolling_State();
		break;

	case EPlatform_State::Adhesive:
		Act_For_Adhesive_State();
		break;

	case EPlatform_State::Expanding:
		Act_For_Expanding_State();
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
			X_Pos = Roll_In_Platform_End_X_Pos - 1;
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
void AsPlatform::Act_For_Adhesive_State()
{
	switch (Platform_State.Adhesive)
	{
	case EPlatform_Substate_Adhesive::Init:
		if (Adhesive_Spot_Height_Ratio < Max_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio += Step_Adhesive_Spot_Height_Ratio;
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Max_Adhesive_Spot_Height_Ratio;
			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Active;
		}	
		Redraw(false);
		break;

	case EPlatform_Substate_Adhesive::Finalize:
		if (Adhesive_Spot_Height_Ratio > Min_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio -= Step_Adhesive_Spot_Height_Ratio;
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Unknown;
			Set_State(EPlatform_Substate_Regular::Normal);
		}

		Redraw(false);
		break;
	}	
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act_For_Expanding_State()
{
	switch (Platform_State.Expanding)
	{
	case EPlatform_Substate_Expanding::Init:
		if (Width < Max_Expanding_Width)
		{
			X_Pos -= Step_Expanding_Width / 2.0;
			Width += Step_Expanding_Width;
		}
		else
		{
			Width = Max_Expanding_Width;
			Platform_State.Expanding = EPlatform_Substate_Expanding::Active;
		}	
		Redraw(false);
		break;

	case EPlatform_Substate_Expanding::Finalize:
		if (Width > Min_Expanding_Width)
		{
			X_Pos += Step_Expanding_Width / 2.0;
			Width -= Step_Expanding_Width;
		}
		else
		{
			Width = Min_Expanding_Width;
			Platform_State.Expanding = EPlatform_Substate_Expanding::Unknown;
			Set_State(EPlatform_Substate_Regular::Normal);
		}

		Redraw(false);
		break;
	}	
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

	/*case EPS_Pre_Meltdown:
		Draw_Normal_State(hdc, paint_area);
		Set_State(EPlatform_State::Meltdown);
		break;*/

	case EPlatform_State::Meltdown:
		if (Platform_State.Meltdown == EPlatform_Substate_Meltdown::Active)
			Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPlatform_State::Rolling:
		Draw_Rolling_State(hdc, paint_area);
		break;

	case EPlatform_State::Adhesive:
		Draw_Adhesive_State(hdc, paint_area);
		break;

	case EPlatform_State::Expanding:
		Draw_Expanding_State(hdc, paint_area);
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

	if (Platform_State == EPlatform_State::Adhesive and Platform_State.Adhesive == EPlatform_Substate_Adhesive::Active and Speed != 0)
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

	double circle_radius = Circle_Size / 2;
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

	if (new_state == Platform_State and Platform_State == EPlatform_State::Adhesive)
		if (Platform_State.Adhesive == EPlatform_Substate_Adhesive::Finalize)
			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Init;

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

	/*case EPS_Pre_Meltdown:
		Speed = 0.0;
		break;*/

	case EPlatform_State::Meltdown:
		Speed = 0.0;
		Platform_State.Meltdown = EPlatform_Substate_Meltdown::Init;

		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(int);
		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
		break;

	case EPlatform_State::Adhesive:
		switch (Platform_State.Adhesive)
		{
		case EPlatform_Substate_Adhesive::Unknown:
			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Init;
			Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
			break;

		case EPlatform_Substate_Adhesive::Finalize:
			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Init;
		}	
		break;

	case EPlatform_State::Expanding:

		switch (Platform_State.Expanding)
		{
		case EPlatform_Substate_Expanding::Unknown:
			Platform_State.Expanding = EPlatform_Substate_Expanding::Init;
			Width = Min_Expanding_Width;
			break;

		case EPlatform_Substate_Expanding::Finalize:
			Platform_State.Expanding = EPlatform_Substate_Expanding::Init;
		}	

		break;
	}

	Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	if (Platform_State == EPlatform_State::Regular and Platform_State.Regular == new_regular_state)
		return;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		if (Platform_State == EPlatform_State::Adhesive)
		{
			if (Platform_State.Adhesive == EPlatform_Substate_Adhesive::Active)
				while (Ball_Set->Release_Next_Ball() )
				{
				}

			Platform_State.Adhesive = EPlatform_Substate_Adhesive::Finalize;
			return;
		}

		if (Platform_State == EPlatform_State::Expanding)
		{
			if (Platform_State.Expanding == EPlatform_Substate_Expanding::Active or Platform_State.Expanding == EPlatform_Substate_Expanding::Init)
			{
				Platform_State.Expanding =  EPlatform_Substate_Expanding::Finalize;
				return;	
			}
		}
	}

	Platform_State = EPlatform_State::Regular;
	Platform_State.Regular = new_regular_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State()
{
	return Platform_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
	Highlight_Color.Select_Pen(hdc);

	Arc(hdc,
		x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
		x + (Circle_Size - 1) * AsConfig::Global_Scale, y + (Circle_Size - 1) * AsConfig::Global_Scale,
		x + (Circle_Size / 2) * AsConfig::Global_Scale, y,
		x, y + (Circle_Size / 2) * AsConfig::Global_Scale);
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
	//Ellipse(hdc, (x + Width - Circle_Size) * d_scale, y * d_scale, (x + Width) * d_scale, (y + Circle_Size) * d_scale);
	Ellipse(hdc, int( (x + Inner_Width + 1) * d_scale), y * scale, int( (x + Inner_Width + Circle_Size + 1) * d_scale - 1), (y + Circle_Size) * scale - 1);

	//draw inner part
	Platform_Inner_Color.Select(hdc);

	inner_rect.left = int( (x + 4) * d_scale);
	inner_rect.top = (y + 1) * scale;
	inner_rect.right = int( (x + 4 + Inner_Width) * d_scale);
	inner_rect.bottom = (y + 1 + Inner_Height) * scale;

	AsConfig::Round_Rect(hdc, inner_rect, Inner_Height);

	//draw highlight
	Draw_Circle_Highlight(hdc, int(x * d_scale), y * scale);

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

	if (update_rect)
	{
		Prev_Platform_Rect = Platform_Rect;

		if (Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
			platform_width = Circle_Size;
		else
			platform_width = int (Width);

		Platform_Rect.left = int(X_Pos * AsConfig::D_Global_Scale);
		Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
		Platform_Rect.right = int(Platform_Rect.left + platform_width * AsConfig::D_Global_Scale);
		Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

		if (Platform_State == EPlatform_State::Meltdown)
		{
			Prev_Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + AsConfig::Global_Scale;
		}
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
	XFORM xform, prev_xform;
	double rotation_angle;

	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, (int)x, y, (int)x + roller_size - 1, y + roller_size);

	GetWorldTransform(hdc, &prev_xform);

	rotation_angle = 2.0 * M_PI * Rolling_Step / (double)Max_Rolling_Step;

	xform.eM11 = (FLOAT)cos(rotation_angle); xform.eM12 = (FLOAT)-sin(rotation_angle);
	xform.eM21 = (FLOAT)sin(rotation_angle); xform.eM22 = (FLOAT)cos(rotation_angle);
	xform.eDx = (FLOAT)(x + roller_size / 2);
	xform.eDy = (FLOAT)(y + roller_size / 2);

	SetWorldTransform(hdc, &xform);
	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, -1, -roller_size / 2, 1, roller_size / 2);

	SetWorldTransform(hdc, &prev_xform);

	Draw_Circle_Highlight(hdc, (int)x, y);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Adhesive_State(HDC hdc, RECT &paint_area)
{
	Draw_Normal_State(hdc, paint_area);

	AsConfig::White_Color.Select(hdc);
	AsConfig::BG_Color.Select_Pen(hdc);

	Draw_Adhesive_Spot(hdc, 0, 7, 4);
	Draw_Adhesive_Spot(hdc, 5, 5, 5);
	Draw_Adhesive_Spot(hdc, 7, 9, 5);

	AsConfig::White_Color.Select(hdc);

	Draw_Adhesive_Spot(hdc, 0, 7, 3);
	Draw_Adhesive_Spot(hdc, 5, 5, 4);
	Draw_Adhesive_Spot(hdc, 7, 9, 4);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Adhesive_Spot(HDC hdc, int x_offset, int width, int height)
{
	RECT spot_rect;

	int x = (int)((X_Pos + 6.0) * AsConfig::D_Global_Scale);
	int y = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;

	int spot_height = int( (double)height * AsConfig::D_Global_Scale * Adhesive_Spot_Height_Ratio);

	spot_rect.left = x + x_offset * AsConfig::Global_Scale - 1;
	spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale + 1;
	spot_rect.top = y - spot_height;
	spot_rect.bottom = y + spot_height - 1;

	Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right, spot_rect.bottom, x, y, spot_rect.right, y);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_State(HDC hdc, RECT &paint_area)
{
	double d_scale = AsConfig::D_Global_Scale;
	int scale = AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos;
	double x = X_Pos;
	int ellipse_offset = 4;
	int ellipse_size = 5;
	double middle_pos;
	double ratio;
	int x_offset;
	RECT intersection_rect;
	RECT inner_rect;

	if (! IntersectRect(&intersection_rect, &Platform_Rect, &paint_area) )
		return;

	middle_pos = x + Width / 2;

	inner_rect.left = int( (middle_pos - Expanding_Platform_Inner_Width / 2.0) * d_scale);
	inner_rect.right = int( (middle_pos + Expanding_Platform_Inner_Width / 2.0) * d_scale);
	inner_rect.top = (y + 1) * scale;
	inner_rect.bottom = (y + 1 + Inner_Height) * scale - 1;

	Platform_Rect.left = int(x * d_scale);
	Platform_Rect.right = int( (x + Width) * d_scale);
	Platform_Rect.top = y * scale;
	Platform_Rect.bottom = (y + Circle_Size) * scale;

	//0.Side parts
	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, Platform_Rect.left, Platform_Rect.top, Platform_Rect.left + Circle_Size * scale - 1, Platform_Rect.bottom - 1);
	Ellipse(hdc, Platform_Rect.right - Circle_Size * scale, Platform_Rect.top, Platform_Rect.right - 1, Platform_Rect.bottom - 1);
	Draw_Circle_Highlight(hdc, int(x * d_scale), y * scale);

	//1.Perforations
	AsConfig::BG_Color.Select(hdc);
	Ellipse(hdc, Platform_Rect.left + ellipse_offset * scale, (y + 1) * scale, Platform_Rect.left + (ellipse_offset + ellipse_size) * scale - 1, (y + 1 + ellipse_size) * scale - 1);
	Ellipse(hdc, Platform_Rect.right - (ellipse_offset + ellipse_size) * scale, (y + 1) * scale, Platform_Rect.right - ellipse_offset * scale - 1, (y + 1 + ellipse_size) * scale - 1);

	//2.Arcs;
	Truss_Expanding_Color.Select_Pen(hdc);
	Arc(hdc, Platform_Rect.left + ellipse_offset * scale, (y + 1) * scale, Platform_Rect.left + (ellipse_offset + ellipse_size) * scale - 1, (y + 1 + ellipse_size) * scale - 1,
		Platform_Rect.left + ellipse_offset * scale, (y + 1) * scale, Platform_Rect.left + ellipse_offset * scale, (y + 1 + ellipse_size) * scale - 1);

	Arc(hdc, Platform_Rect.right - (ellipse_offset + ellipse_size) * scale, (y + 1) * scale, Platform_Rect.right - ellipse_offset * scale - 1, (y + 1 + ellipse_size) * scale - 1,
		Platform_Rect.right - ellipse_offset * scale - 1, (y + 1 + ellipse_size) * scale - 1,
		Platform_Rect.right - ellipse_offset * scale - 1, (y + 1) * scale);

	//3.Inner part
	Platform_Inner_Color.Select(hdc);
	Rectangle(hdc, inner_rect.left, inner_rect.top, inner_rect.right, inner_rect.bottom);

	//4.Truss
	ratio = Width / Max_Expanding_Width;
	Draw_Expanding_Truss(hdc, x, y, ratio);
	x_offset = int( ( (Max_Expanding_Width - Expanding_Platform_Inner_Width - ellipse_offset * 2.0) / 2.0 + Expanding_Platform_Inner_Width) * ratio - 1);
	x += x_offset;
	Draw_Expanding_Truss(hdc, x, y, ratio);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Truss(HDC hdc, double x, int y, double ratio)
{
	int scale = AsConfig::Global_Scale;
	double d_scale = AsConfig::D_Global_Scale;

	Truss_Expanding_Color.Select_Pen(hdc);

	MoveToEx(hdc, int( (x + 5) * scale + 1), (y + 2) * scale - 1, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 5) * scale + 1);
	LineTo(hdc, int( (x + 14 * ratio) * d_scale - 1), (y + 2) * scale - 1);

	MoveToEx(hdc, int( (x + 5) * d_scale + 1), (y + 5) * scale, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 1) * scale + 1);
	LineTo(hdc, int ( (x + 14 * ratio) * d_scale - 1), (y + 5) * scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left, bool key_down)
{
	/*if (Platform_State != EPS_Normal)
		return;*/

	if (! ( Has_State(EPlatform_Substate_Regular::Normal) or Platform_State == EPlatform_State::Adhesive or Platform_State == EPlatform_State::Expanding) ) 
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
bool AsPlatform::Hit_By(AFalling_Letter *falling_letter)
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
}
//------------------------------------------------------------------------------------------------------------