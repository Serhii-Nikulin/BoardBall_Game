#include <Windows.h>
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
char ALevel::Level_01[ALevel::Level_Height][ALevel::Level_Width] = {
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//1
		0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//2
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//3
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//4
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,//5
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,//6
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,//7
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,//8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//12
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//13
};
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
	: Brick_Blue_Pen(0), Brick_Blue_Brush(0), Brick_Red_Pen(0), Brick_Red_Brush(0), Letter_Pen(0), Level_Rect{}, Active_Brick(EBT_Blue), Current_Brick_Left_X(0), Current_Brick_Right_X(0), Current_Brick_Top_Y(0), Current_Brick_Low_Y(0)
{}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, Brick_Blue_Pen, Brick_Blue_Brush);
	AsConfig::Create_Pen_Brush(AsConfig::Red_Brick_Color, Brick_Red_Pen, Brick_Red_Brush);

	Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		return;

	for (i = 0; i < Level_Height; ++i)
		for (j = 0; j < Level_Width; ++j)
			Draw_Brick(hdc, AsConfig::Level_X_Offset + j * Cell_Width, AsConfig::Level_Y_Offset + Cell_Height * i, static_cast<EBrick_Type>(Level_01[i][j]));

	Active_Brick.Draw(hdc);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_Blue:
		pen = Brick_Blue_Pen;
		brush = Brick_Blue_Brush;
		break;

	case EBT_Red:
		pen = Brick_Red_Pen;
		brush = Brick_Red_Brush;
		break;

	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	bool switch_color;
	double offset;
	double rotation_angle;
	int back_part_offset;
	int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xform, prev_xform;

	if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
		return;

	rotation_step %= 16;

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI * rotation_step / 16.0;
	else
		rotation_angle = 2.0 * M_PI * (8 - rotation_step) / 16.0;

	if (rotation_step > 4 and rotation_step <= 12)
	{
		if (brick_type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
	}
	else
	{
		if (brick_type == EBT_Blue)
			switch_color = false;
		else
			switch_color = true;
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (rotation_step == 4 or rotation_step == 12)
	{
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height);

		//Front side
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height + AsConfig::Global_Scale - 1);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);
		GetWorldTransform(hdc, &prev_xform);

		xform.eM11 = (FLOAT)1; xform.eM12 = (FLOAT)0;
		xform.eM21 = (FLOAT)0; xform.eM22 = (FLOAT)cos(rotation_angle);
		xform.eDx = (FLOAT)x;
		xform.eDy = (FLOAT)y + brick_half_height;

		SetWorldTransform(hdc, &xform);

		offset = 3.0 * (1.0f - fabs(cos(rotation_angle))) * AsConfig::Global_Scale;
		back_part_offset = (int)round(offset);

		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, +brick_half_height - back_part_offset);

		//Front side
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -(int)brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, +(int)brick_half_height);

		if (rotation_step > 4 and rotation_step < 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc,
					(AsConfig::Brick_Width - (AsConfig::Brick_Height - 2)) * AsConfig::Global_Scale / 2,
					-(AsConfig::Brick_Height - 2) * AsConfig::Global_Scale / 2,
					(AsConfig::Brick_Width + (AsConfig::Brick_Height - 2)) * AsConfig::Global_Scale / 2,
					+(AsConfig::Brick_Height - 2) * AsConfig::Global_Scale / 2);
			}
		}

		SetWorldTransform(hdc, &prev_xform);
	}
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
	if (is_switch_color)
	{
		front_pen = Brick_Red_Pen;
		front_brush = Brick_Red_Brush;
		back_pen = Brick_Blue_Pen;
		back_brush = Brick_Blue_Brush;
	}
	else
	{
		front_pen = Brick_Blue_Pen;
		front_brush = Brick_Blue_Brush;
		back_pen = Brick_Red_Pen;
		back_brush = Brick_Red_Brush;
	}
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	int i, j;
	double direction = ball->Get_Direction();
	int min_y, max_y, min_x, max_x;

	if (next_y_pos > AsConfig::Border_Y_Offset + (Level_Height - 1) * Cell_Height + Cell_Height)
		return false;

	min_y = (next_y_pos - ball->Radius - AsConfig::Level_Y_Offset) / Cell_Height;
	max_y = (next_y_pos + ball->Radius - AsConfig::Level_Y_Offset) / Cell_Height;

	min_x = (next_x_pos - ball->Radius - AsConfig::Level_X_Offset) / Cell_Width;
	max_x = (next_x_pos + ball->Radius - AsConfig::Level_X_Offset) / Cell_Width;

	for (i = max_y; i >= min_y; --i)//y
	//for (i = ALevel::Level_Height - 1; i >= 0; --i)
	{
		for (j = min_x; j <= max_x; ++j)//x
		{
			if (Level_01[i][j] == 0)
				continue;

			Current_Brick_Top_Y = AsConfig::Level_Y_Offset + i * ALevel::Cell_Height;
			Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;
			
			Current_Brick_Left_X = AsConfig::Level_X_Offset + j * ALevel::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

			if (Is_Horizontal_Hit_First(next_x_pos, next_y_pos))
			{
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
					return true;
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
					return true;
			}
			else
			{
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
					return true;
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
					return true;
			}
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos)
{
	double min_distance_to_horizon, min_distance_to_vertical, another_distance;

	min_distance_to_horizon = fabs(next_x_pos - Current_Brick_Left_X);
	another_distance = fabs(next_x_pos - Current_Brick_Right_X);
	min_distance_to_horizon = min_distance_to_horizon < another_distance ? min_distance_to_horizon : another_distance;

	min_distance_to_vertical = fabs(next_y_pos - Current_Brick_Top_Y);
	another_distance = fabs(next_y_pos - Current_Brick_Low_Y);
	min_distance_to_horizon = min_distance_to_vertical < another_distance ? min_distance_to_vertical : another_distance;

	if (min_distance_to_horizon > min_distance_to_vertical)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball)
{
	double direction = ball->Get_Direction();
	//reflection from right_brick to right
	if (direction > M_PI_2 and direction < M_PI + M_PI_2)
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Right_X, ball->Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x < Level_Width - 1 and Level_01[level_y][level_x + 1] == 0)
			{
				ball->Reflect(false);//from vertical
				return true;
			}
			else
				return false;
		}

	//reflection from left_brick to left
	if (direction > 0 and direction < M_PI_2 or direction > M_PI + M_PI_2 and direction < 2 * M_PI)
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Left_X, ball->Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x > 0 and Level_01[level_y][level_x - 1] == 0)
			{
				ball->Reflect(false);//from vertical
				return true;
			}
			else
				return false;
		}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball)
{
	double direction = ball->Get_Direction();

	//reflection from low_brick
	if (direction > 0 and direction < M_PI)
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Low_Y, ball->Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y < Level_Height - 1 and Level_01[level_y + 1][level_x] == 0)
			{
				ball->Reflect(true);//from horizontal
				return true;
			}
			else
				return false;
		}

	//reflection from top_brick
	if (direction > M_PI and direction < M_PI * 2)
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Top_Y, ball->Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y > 0 and Level_01[level_y - 1][level_x] == 0)
			{
				ball->Reflect(true);//from horizontal
				return true;
			}
			else
				return false;
		}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Hit_Circle_On_Line(double next_pos, double eval_pos, double radius, double min_value, double max_value)
{
	if (eval_pos > radius)
		return false;

	//value * value + eval_pos * eval_pos = radius * radius

	double value = sqrt(radius * radius - eval_pos * eval_pos);
	
	if ((next_pos + value) > min_value and (next_pos - value) < max_value)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------