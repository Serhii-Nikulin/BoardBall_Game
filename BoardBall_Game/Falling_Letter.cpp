#include "Falling_Letter.h"

//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
	:Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Rotation_Step(2), Got_Hit(false)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = X + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Letter_Cell.bottom = Y + AsConfig::Brick_Height * AsConfig::Global_Scale;
	Prev_Letter_Cell = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{
	bool switch_color;
	double offset;
	double rotation_angle;
	int back_part_offset;
	int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xform, prev_xform;

	if (!(Brick_Type == EBT_Blue || Brick_Type == EBT_Red))
		return;

	Rotation_Step %= 16;

	if (Rotation_Step < 8)
		rotation_angle = 2.0 * M_PI * Rotation_Step / 16.0;
	else
		rotation_angle = 2.0 * M_PI * (8 - Rotation_Step) / 16.0;

	if (Rotation_Step > 4 and Rotation_Step <= 12)
	{
		if (Brick_Type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
	}
	else
	{
		if (Brick_Type == EBT_Blue)
			switch_color = false;
		else
			switch_color = true;
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (Rotation_Step == 4 or Rotation_Step == 12)
	{
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, X, Y + brick_half_height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height);

		//Front side
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height + AsConfig::Global_Scale - 1);
	}
	else
	{
		GetWorldTransform(hdc, &prev_xform);

		xform.eM11 = (FLOAT)1; xform.eM12 = (FLOAT)0;
		xform.eM21 = (FLOAT)0; xform.eM22 = (FLOAT)cos(rotation_angle);
		xform.eDx = (FLOAT)X;
		xform.eDy = (FLOAT)Y + brick_half_height;

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

		if (Rotation_Step > 4 and Rotation_Step < 12)
		{
			if (Letter_Type == ELT_O)
			{
				SelectObject(hdc, AsConfig::Letter_Pen);
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
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
	if (is_switch_color)
	{
		front_pen = AsConfig::Brick_Red_Pen;
		front_brush = AsConfig::Brick_Red_Brush;
		back_pen = AsConfig::Brick_Blue_Pen;
		back_brush = AsConfig::Brick_Blue_Brush;
	}
	else
	{
		front_pen = AsConfig::Brick_Blue_Pen;
		front_brush = AsConfig::Brick_Blue_Brush;
		back_pen = AsConfig::Brick_Red_Pen;
		back_brush = AsConfig::Brick_Red_Brush;
	}
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
	Prev_Letter_Cell = Letter_Cell;

	Y += AsConfig::Global_Scale;
	Letter_Cell.top += AsConfig::Global_Scale;
	Letter_Cell.bottom += AsConfig::Global_Scale;

	Rotation_Step += 1;

	InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);
		Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right - 1, Prev_Letter_Cell.bottom - 1);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
		Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	if (Got_Hit or (Letter_Cell.top > AsConfig::Max_Y_Pos * AsConfig::Global_Scale) )
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Get_Letter_Cell(RECT &rect)
{
	rect = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------