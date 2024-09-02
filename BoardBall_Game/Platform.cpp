#include "Platform.h"

int AsPlatform::Meltdown_Platform_Y_Pos[Normal_Width];
const double AsPlatform::Max_Adhesive_Spot_Height_Ratio = 1.0;
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform() :
	Platform_State(EPS_Missing), Platform_Moving_State(EPMS_Stop), Inner_Width(Normal_Inner_Width), Rolling_Step(0), Width(28), X_Pos(AsConfig::Start_Ball_Position_On_Platform - Width / 2),
	Prev_Platform_Rect{}, Platform_Rect{},	Normal_Platform_Image_Width (28 * AsConfig::Global_Scale), Ball_Set(0),
	Normal_Platform_Image_Height(Height * AsConfig::Global_Scale),Normal_Platform_Image(0), Platform_Inner_Color(237, 38, 36), 
	Platform_Circle_Color(63, 72, 204), Highlight_Color(255, 255, 255), Speed(0.0), Left_Key_Down(false), Right_Key_Down(false), Adhesive_Spot_Height_Ratio(0.0)
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

	if (Platform_State == EPS_Adhesive)
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
	if (Platform_State == EPMS_Stop)
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
	case EPS_Meltdown:
	case EPS_Roll_In:
	case EPS_Expand_Roll_In:
		Redraw();
		break;
	case EPS_Adhesive_Init:
		if (Adhesive_Spot_Height_Ratio < Max_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio += 0.02;
			Redraw(false);

		}
		else
		{
			Adhesive_Spot_Height_Ratio = Max_Adhesive_Spot_Height_Ratio;
			Set_State(EPS_Adhesive);
		}	
		break;

	case EPS_Adhesive_Finalize:
		if (Adhesive_Spot_Height_Ratio > 0.0)
		{
			Adhesive_Spot_Height_Ratio -= 0.02;
			Redraw(false);

		}
		else
		{
			Adhesive_Spot_Height_Ratio = 0.0;
			Set_State(EPS_Normal);
		}	
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
	case EPS_Ready:
	case EPS_Normal:
		Draw_Normal_State(hdc, paint_area);
		break;

	case EPS_Pre_Meltdown:
		Draw_Normal_State(hdc, paint_area);
		Set_State(EPS_Meltdown);
		break;

	case EPS_Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPS_Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;

	case EPS_Expand_Roll_In:
		Draw_Expandig_Roll_In_State(hdc, paint_area);
		break;

	case EPS_Adhesive_Init:
	case EPS_Adhesive:
	case EPS_Adhesive_Finalize:
		Draw_Adhesive_State(hdc, paint_area);
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
	case EPS_Ready:
	case EPS_Normal:
	case EPS_Pre_Meltdown:
	case EPS_Roll_In:
	case EPS_Expand_Roll_In:
	case EPS_Adhesive_Init:
	case EPS_Adhesive:
	case EPS_Adhesive_Finalize:
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

	if (Platform_Moving_State == EPMS_Moving_Left)
	{
		if (X_Pos < AsConfig::Border_X_Offset)
		{
			X_Pos = AsConfig::Border_X_Offset;
			Speed = 0.0;
		}
	}
	else if (Platform_Moving_State == EPMS_Moving_Right)
	{
		if (X_Pos > AsConfig::Max_X_Pos - Width + 1)
		{
			X_Pos = AsConfig::Max_X_Pos - Width + 1;
			Speed = 0.0;
		}
	}

	if (Platform_State == EPS_Adhesive and Speed != 0)
	{
		if (Platform_Moving_State == EPMS_Moving_Left)
		{
			Ball_Set->Shift_By_Platform(M_PI, fabs(Speed), max_speed);
		}
		else if (Platform_Moving_State == EPMS_Moving_Right)
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

	if (Platform_State == new_state)
		return;

	switch (new_state)
	{
	case EPS_Roll_In:
		X_Pos = AsConfig::Max_X_Pos;
		Rolling_Step = Max_Rolling_Step - 1;
		Inner_Width = 0;
		break;
	
	case EPS_Pre_Meltdown:
		Speed = 0.0;
		break;

	case EPS_Meltdown:
		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(int);
		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
		break;

	case EPS_Adhesive_Init:
		if (Platform_State == EPS_Adhesive)
			return;

		if (Platform_State == EPS_Adhesive_Finalize)
			break;

		Adhesive_Spot_Height_Ratio = 0.2;
		break;

	case EPS_Adhesive:
		break;

	case EPS_Adhesive_Finalize:
		while (Ball_Set->Release_Next_Ball() )
		{
		}
		break;
	}

	Platform_State = new_state;
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

	if (Normal_Platform_Image == 0 and Platform_State == EPS_Ready)
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
		Platform_State = EPS_Missing;
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

		if (Platform_State == EPS_Roll_In)
			platform_width = Circle_Size;
		else
			platform_width = Width;

		Platform_Rect.left = int(X_Pos * AsConfig::D_Global_Scale);
		Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
		Platform_Rect.right = int(Platform_Rect.left + platform_width * AsConfig::D_Global_Scale);
		Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

		if (Platform_State == EPS_Meltdown)
		{
			Prev_Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + AsConfig::Global_Scale;
		}
	}

	InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	double x = (int)X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int roller_size = Circle_Size * AsConfig::Global_Scale;
	XFORM xform, prev_xform;
	double rotation_angle;

	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, (int)x, y, (int)x + roller_size - 1, y + roller_size);

	GetWorldTransform(hdc, &prev_xform);

	Rolling_Step %= Max_Rolling_Step;
	rotation_angle = 2.0 * M_PI * Rolling_Step / (double)Max_Rolling_Step;
	Rolling_Step += 1;

	xform.eM11 = (FLOAT)cos(rotation_angle); xform.eM12 = (FLOAT)-sin(rotation_angle);
	xform.eM21 = (FLOAT)sin(rotation_angle); xform.eM22 = (FLOAT)cos(rotation_angle);
	xform.eDx = (FLOAT)(x + roller_size / 2);
	xform.eDy = (FLOAT)(y + roller_size / 2);

	SetWorldTransform(hdc, &xform);
	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, -1, -roller_size / 2, 1, roller_size / 2);

	SetWorldTransform(hdc, &prev_xform);

	Draw_Circle_Highlight(hdc, (int)x, y);

	X_Pos -= Rolling_Platform_Speed;

	if (X_Pos <= Roll_In_Platform_End_X_Pos)
	{
		X_Pos = Roll_In_Platform_End_X_Pos - 1;
		Platform_State = EPS_Expand_Roll_In;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expandig_Roll_In_State(HDC hdc, RECT paint_area)
{
	if (Inner_Width < Normal_Inner_Width)
	{
		Inner_Width += 2;
		X_Pos -= 1;
		Platform_Rect.left = (int)X_Pos;
	}
	else
	{
		Inner_Width = Normal_Inner_Width;
		Platform_State = EPS_Ready;
	}

	Draw_Normal_State(hdc, paint_area);
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
void AsPlatform::Move(bool to_left, bool key_down)
{
	/*if (Platform_State != EPS_Normal)
		return;*/

	if ( !(Platform_State == EPS_Normal or Platform_State == EPS_Adhesive or Platform_State == EPS_Adhesive_Init or Platform_State == EPS_Adhesive_Finalize) )
		return;

	if (to_left)
		Left_Key_Down = key_down;
	else
		Right_Key_Down = key_down;

	if (Left_Key_Down and Right_Key_Down)
		return;

	if (!Left_Key_Down and !Right_Key_Down)
	{
		Platform_Moving_State = EPMS_Stop;
		Speed = 0.0;
		return;
	}

	if (Left_Key_Down)
	{
		Platform_Moving_State = EPMS_Moving_Left;	
		Speed = -X_Step;
	}

	if(Right_Key_Down)
	{
		Platform_Moving_State = EPMS_Moving_Right;	
		Speed = X_Step;
	}
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

	switch (Platform_State)
	{
	case EPS_Ready:
		Ball_Set->Release_From_Platform();
		Set_State(EPS_Normal);
		break;

	case EPS_Adhesive:
		Ball_Set->Release_Next_Ball();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------