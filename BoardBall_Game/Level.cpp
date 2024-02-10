#include <Windows.h>
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
char ALevel::Level_01[ALevel::Level_Height][ALevel::Level_Width] = {
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//1
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//2
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//3
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//4
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//5
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//6
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//7
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//11
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//12
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//13
};

//------------------------------------------------------------------------------------------------------------
ALevel::ALevel():
	Brick_Blue_Pen{}, Brick_Blue_Brush{}, Brick_Red_Pen{}, Brick_Red_Brush{}, Letter_Pen{}, Level_Rect{}
{}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(63, 72, 204, Brick_Blue_Pen, Brick_Blue_Brush);
	AsConfig::Create_Pen_Brush(237, 38, 36, Brick_Red_Pen, Brick_Red_Brush);

	Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HWND hwnd, HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT intersection_rect;
	RECT brick_rect;
	HPEN pen;
	HBRUSH brush;
	

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
		rotation_angle = 2.0 * M_PI / 16 * rotation_step;
	else
		rotation_angle = 2.0 * M_PI / 16 * (8 - rotation_step);

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
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction)
{
	int i, j;
	int brick_y_pos = AsConfig::Level_Y_Offset + (ALevel::Level_Height - 1) * ALevel::Cell_Height + AsConfig::Brick_Height;

	for (i = ALevel::Level_Height - 1; i >= 0; --i)
	{
		for (j = 0; j < ALevel::Level_Width; ++j)
		{
			if (Level_01[i][j] == 0)
				continue;

			if (brick_y_pos > next_y_pos)
			{
				ball_direction = -ball_direction;
				next_y_pos = brick_y_pos + (brick_y_pos - next_y_pos);
				break;
			}
		}
		brick_y_pos -= ALevel::Cell_Height;
	}
}
//------------------------------------------------------------------------------------------------------------