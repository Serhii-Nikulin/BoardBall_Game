#include "Platform.h"

int AsPlatform::Meltdown_Platform_Y_Pos[Normal_Width];
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform() :
	Platform_State(EPS_Missing), Inner_Width(Normal_Inner_Width), Rolling_Step(0), Width(28), X_Pos(103 - Width / 2), X_Step(2 * AsConfig::Global_Scale),
	Prev_Platform_Rect{}, Platform_Rect{},	Normal_Platform_Image_Width (28 * AsConfig::Global_Scale), Normal_Platform_Image_Height(Height * AsConfig::Global_Scale),
	Normal_Platform_Image(0), Platform_Inner_Color(237, 38, 36), Platform_Circle_Color(63, 72, 204), Highlight_Color(255, 255, 255)
{}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	double inner_top_y;
	double inner_low_y;
	double inner_left_x;
	double inner_right_x;

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
	{
		ball->Set_State(EBS_Off_Parachute);
	}

	return true;
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

	double ball_left_x = next_x_pos - ball->Radius;
	double ball_right_x = next_x_pos + ball->Radius;
	double ball_top_y = next_y_pos - ball->Radius;
	double ball_low_y = next_y_pos + ball->Radius;

	platform_ball_x = X_Pos + x_offset + circle_radius;
	platform_ball_y = AsConfig::Platform_Y_Pos + circle_radius;
	dx = next_x_pos - platform_ball_x;
	dy = platform_ball_y - next_y_pos;

	if ((ball_right_x > X_Pos + x_offset and ball_left_x < X_Pos + Circle_Size - 1 + x_offset) 
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
			ball->Set_State(EBS_Normal, (int)(next_x_pos + AsConfig::Global_Scale * cos(direction)), (int)(next_y_pos - AsConfig::Global_Scale * sin(direction)), direction);

		ball->prev_angle_to_normal = angle_to_normal;
		return true;
	}
	return false;
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
	}
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

	case EPS_Meltdown:
		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(int);
		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
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

	case EPS_Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPS_Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;

	case EPS_Expand_Roll_In:
		Draw_Expandig_Roll_In_State(hdc, paint_area);
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc)
{
	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
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
	int i, j;
	int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;
	int offset = 0;
	RECT intersection_rect{};
	RECT inner_rect{};

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect))
		Clear_BG(hdc);

	//draw side parts
	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale - 1, (y + Circle_Size) * AsConfig::Global_Scale - 1);
	//Ellipse(hdc, (x + Width - Circle_Size) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Width) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
	Ellipse(hdc, (x + Inner_Width + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Inner_Width + Circle_Size + 1) * AsConfig::Global_Scale - 1, (y + Circle_Size) * AsConfig::Global_Scale - 1);

	//draw inner part
	Platform_Inner_Color.Select(hdc);

	inner_rect.left = (x + 4) * AsConfig::Global_Scale;
	inner_rect.top = (y + 1) * AsConfig::Global_Scale;
	inner_rect.right = (x + 4 + Inner_Width) * AsConfig::Global_Scale;
	inner_rect.bottom = (y + 1 + Inner_Height) * AsConfig::Global_Scale;

	AsConfig::Round_Rect(hdc, inner_rect, Inner_Height);

	//draw highlight
	Draw_Circle_Highlight(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale);

	if (Normal_Platform_Image == 0 and Platform_State == EPS_Ready)
	{
		Normal_Platform_Image = new int[Normal_Platform_Image_Height * Normal_Platform_Image_Width];
		x *= AsConfig::Global_Scale;
		y *= AsConfig::Global_Scale;

		for (i = 0; i < Normal_Platform_Image_Height; i++)
			for (j = 0; j < Normal_Platform_Image_Width; j++)
				Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
	}
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
void AsPlatform::Redraw()
{
	int platform_width;
	Prev_Platform_Rect = Platform_Rect;

	if (Platform_State == EPS_Roll_In)
		platform_width = Circle_Size;
	else
		platform_width = Width;

	Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
	Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

	if (Platform_State == EPS_Meltdown)
	{
		Prev_Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + AsConfig::Global_Scale;
	}

	InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	int x = X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int roller_size = Circle_Size * AsConfig::Global_Scale;
	XFORM xform, prev_xform;
	double rotation_angle;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect))
		Clear_BG(hdc);

	Platform_Circle_Color.Select(hdc);
	Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size);

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

	Draw_Circle_Highlight(hdc, x, y);

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
		Platform_Rect.left = X_Pos;
	}
	else
	{
		Inner_Width = Normal_Inner_Width;
		Platform_State = EPS_Ready;
	}

	Draw_Normal_State(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left)
{
	if (!(Platform_State == EPS_Normal))
		return;

	if (to_left)
	{
		X_Pos -= X_Step;
		
		if (X_Pos < AsConfig::Border_X_Offset)
			X_Pos = AsConfig::Border_X_Offset;

		Redraw();
	}
	else
	{
		X_Pos += X_Step;

		if (X_Pos > AsConfig::Max_X_Pos - Width + 1)
			X_Pos = AsConfig::Max_X_Pos - Width + 1;

		Redraw();
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