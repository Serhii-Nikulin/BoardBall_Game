#include "Platform.h"

//AsPlatform
int AsPlatform::Meltdown_Platform_Y_Pos[AsConfig::Platform_Normal_Width * AsConfig::Global_Scale];
AHit_Checker_List AsPlatform::Hit_Checker_List;
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform() :
	Inner_Width(Normal_Inner_Width), Rolling_Step(0), Width(28), X_Pos(AsConfig::Start_Ball_Position_On_Platform - Width / 2), Prev_Platform_Rect{}, Platform_Rect{}, Normal_Platform_Image_Width (28 * AsConfig::Global_Scale), 
	Ball_Set(0), Normal_Platform_Image_Height(Height * AsConfig::Global_Scale),Normal_Platform_Image(0), Platform_Inner_Color(237, 38, 36), Platform_Circle_Color(63, 72, 204), Highlight_Color(255, 255, 255), 
	Truss_Expanding_Color(Platform_Inner_Color, AsConfig::Global_Scale), Gun_Color(Highlight_Color, AsConfig::Global_Scale), Speed(0.0), Last_Redraw_Timer_Tick(0), Left_Key_Down(false), Right_Key_Down(false), 
	Platform_Adhesive(Platform_State), Platform_Expanding(Platform_State), Platform_Laser(Platform_State)
{}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(AsLaser_Beam_Set *laser_beam_set, AsBall_Set *ball_set)
{
	Ball_Set = ball_set;
	Platform_Expanding.Init(Platform_Inner_Color, Platform_Circle_Color, Highlight_Color);
	Platform_Laser.Init(laser_beam_set, Platform_Inner_Color, Platform_Circle_Color, Highlight_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{
	double inner_top_y;
	double inner_low_y;
	double inner_left_x;
	double inner_right_x;

	double x_pos, y_pos;
	double speed;

	double circle_radius;
	double platform_ball_x, platform_ball_y;

	if (next_y_pos < AsConfig::Platform_Y_Pos)
		return false;

	circle_radius = AsConfig::Platform_Circle_Size / 2.0;
	platform_ball_x = X_Pos + circle_radius;
	platform_ball_y = AsConfig::Platform_Y_Pos + circle_radius;

	if (AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, platform_ball_x, platform_ball_y, circle_radius, ball) )//from left
		goto got_hit;

	platform_ball_x = X_Pos + circle_radius + (Width - AsConfig::Platform_Circle_Size);

	if (AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, platform_ball_x, platform_ball_y, circle_radius, ball) )//from right
		goto got_hit;

	inner_top_y = AsConfig::Platform_Y_Pos + 1;
	inner_low_y = AsConfig::Platform_Y_Pos + AsConfig::Platform_Inner_Height + 1;
	inner_left_x = double(X_Pos + AsConfig::Platform_Circle_Size - 1 + AsConfig::Ball_Radius);
	inner_right_x = double(X_Pos + Width - AsConfig::Platform_Circle_Size - AsConfig::Ball_Radius);

	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - inner_low_y, AsConfig::Ball_Radius, inner_left_x, inner_right_x))
		{
			ball->Reflect(true);//from platform horizontal
			goto got_hit;
		}
	}
	else
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - inner_top_y, AsConfig::Ball_Radius, inner_left_x, inner_right_x))
		{
			ball->Reflect(true);//from platform horizontal
			goto got_hit;
		}
	}

	return false;

got_hit:

	if (ball->Get_State() == EBall_State::On_Parachute)
		ball->Set_State(EBall_State::Off_Parachute);

	if (Platform_State == EPlatform_State::Adhesive and Platform_State.Adhesive == EPlatform_Transformation::Active)
	{
		speed = ball->Get_Speed();
		ball->Get_Center(x_pos, y_pos);
		ball->Set_State(EBall_State::On_Platform, x_pos, y_pos, ball->Get_Direction() );
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
		if (Platform_Laser.Act(next_state, X_Pos) )
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
		AsTools::Rect(hdc, Prev_Platform_Rect, AsConfig::BG_Color);

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
	double min_platform_x = AsConfig::Border_X_Offset + 1;

	X_Pos += step;

	if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Left)
	{
		if (X_Pos < min_platform_x)
		{
			X_Pos = min_platform_x;
			Speed = 0.0;
		}
	}
	else if (Platform_State.Moving_State == EPlatform_Moving_State::Moving_Right)
	{
		if (X_Pos > AsConfig::Max_X_Pos - Width)
		{
			X_Pos = AsConfig::Max_X_Pos - Width;
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

	Hit_Checker_List.Check_Hit(Platform_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, ABall *ball, double x_offset)
{
	double angle_to_normal;
	double reflect_angle;
	double full_reflect_angle;

	double circle_radius = AsConfig::Platform_Circle_Size / 2.0;
	double platform_ball_x, platform_ball_y;
	double dx, dy;
	double direction;
	double speed;

	double ball_left_x = next_x_pos - AsConfig::Ball_Radius;
	double ball_right_x = next_x_pos + AsConfig::Ball_Radius;
	double ball_top_y = next_y_pos - AsConfig::Ball_Radius;
	double ball_low_y = next_y_pos + AsConfig::Ball_Radius;

	platform_ball_x = X_Pos + x_offset + circle_radius;
	platform_ball_y = AsConfig::Platform_Y_Pos + circle_radius;
	dx = next_x_pos - platform_ball_x;
	dy = platform_ball_y - next_y_pos;

	if ( (ball_right_x > X_Pos + x_offset and ball_left_x < X_Pos + AsConfig::Platform_Circle_Size - 1 + x_offset) 
		and 
		(ball_low_y > AsConfig::Platform_Y_Pos and ball_top_y < AsConfig::Platform_Y_Pos + AsConfig::Platform_Circle_Size - 1) )
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
		}

		if (ball->Get_State() == EBall_State::On_Parachute)
			ball->Set_State(EBall_State::Off_Parachute);
		else
		{
			speed = ball->Get_Speed();
			ball->Set_State(EBall_State::Normal, (int)(next_x_pos + AsConfig::Global_Scale * cos(direction)), (int)(next_y_pos - AsConfig::Global_Scale * sin(direction) ), direction);
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
		Redraw();
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
	Ellipse(hdc, int (x * d_scale), y * scale, int ( (x + AsConfig::Platform_Circle_Size) * d_scale - 1), (y + AsConfig::Platform_Circle_Size) * scale - 1);
	Ellipse(hdc, int( (x + Inner_Width + 1) * d_scale), y * scale, int( (x + Inner_Width + AsConfig::Platform_Circle_Size + 1) * d_scale - 1), (y + AsConfig::Platform_Circle_Size) * scale - 1);

	//draw inner part
	Platform_Inner_Color.Select(hdc);

	inner_rect.left = int( (x + 4) * d_scale);
	inner_rect.top = (y + 1) * scale;
	inner_rect.right = int( (x + 4 + Inner_Width) * d_scale);
	inner_rect.bottom = (y + 1 + AsConfig::Platform_Inner_Height) * scale;

	AsTools::Round_Rect(hdc, inner_rect, AsConfig::Platform_Inner_Height);

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

		y_offset = AsTools::Rand(Meltdown_Speed);

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
		platform_width = AsConfig::Platform_Circle_Size;
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
	
	AsTools::Invalidate_Rect(Prev_Platform_Rect);
	AsTools::Invalidate_Rect(Platform_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect{};
	double x = (int)X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int roller_size = AsConfig::Platform_Circle_Size * AsConfig::Global_Scale;
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
	if (Has_State(EPlatform_Substate_Regular::Ready) )
	{
		if (key_down)
		{
			Ball_Set->Release_From_Platform();
			Set_State(EPlatform_Substate_Regular::Normal);
		}
	}
	else
		if (Platform_State == EPlatform_State::Adhesive)
		{
			if (key_down)
				Ball_Set->Release_Next_Ball();
		}
		else if (Platform_State == EPlatform_State::Laser)			
			Platform_Laser.Fire(key_down);
}
//------------------------------------------------------------------------------------------------------------