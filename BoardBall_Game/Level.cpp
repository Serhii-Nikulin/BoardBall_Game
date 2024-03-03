#include <Windows.h>
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
	:Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Rotation_Step(0)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = X + AsConfig::Cell_Width * AsConfig::Global_Scale;
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
		SetGraphicsMode(hdc, GM_ADVANCED);
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
void AFalling_Letter::Draw(HDC hdc)
{
	RECT intersection_rect;

	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);
	Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right - 1, Prev_Letter_Cell.bottom - 1);
	
	Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	return true;
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
int ALevel::Active_Bricks_Count = 0;
//------------------------------------------------------------------------------------------------------------
char ALevel::Current_Level[Level_Height][Level_Width];
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
char ALevel::Test_Level[Level_Height][Level_Width] = {
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//2
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//8
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,//9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//12
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//13
};
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
	: Level_Rect{}, Active_Brick(EBT_Blue, 0, 0), Current_Brick_Left_X(0), Current_Brick_Right_X(0), Current_Brick_Top_Y(0), Current_Brick_Low_Y(0), Active_Bricks{}
{}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
	Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * Level_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * Level_Height * AsConfig::Global_Scale;

	memset(Current_Level, 0, sizeof(Current_Level));
	memset(Active_Bricks, 0, sizeof(Active_Bricks));
	memset(Falling_Letters, 0, sizeof(Falling_Letters));
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(char level[Level_Height][Level_Width])
{
	memcpy(Current_Level, level, sizeof(Current_Level));
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{
		for (i = 0; i < Level_Height; ++i)
			for (j = 0; j < Level_Width; ++j)
				Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + AsConfig::Cell_Height * i, static_cast<EBrick_Type>(Current_Level[i][j]));
	}

	for (i = 0; i < Max_Active_Bricks_Count; i++)
	{
		if (Active_Bricks[i] and IntersectRect(&intersection_rect, &paint_area, &Active_Bricks[i]->Brick_Rect))
			Active_Bricks[i]->Draw(hdc);
	}

	for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
	{
		if (Falling_Letters[i] and 
			(IntersectRect(&intersection_rect, &paint_area, &Falling_Letters[i]->Letter_Cell) 
			or 
			IntersectRect(&intersection_rect, &paint_area, &Falling_Letters[i]->Prev_Letter_Cell)))
			Falling_Letters[i]->Draw(hdc);
	}
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_Blue:
		pen = AsConfig::Brick_Blue_Pen;
		brush = AsConfig::Brick_Blue_Brush;
		break;

	case EBT_Red:
		pen = AsConfig::Brick_Red_Pen;
		brush = AsConfig::Brick_Red_Brush;
		break;

	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
	int i, j;
	double direction = ball->Get_Direction();
	int min_y, max_y, min_x, max_x;

	if (next_y_pos + ball->Radius > AsConfig::Border_Y_Offset + (Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Cell_Height)
		return false;

	min_y = int(next_y_pos - ball->Radius - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;
	max_y = int(next_y_pos + ball->Radius - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;

	min_x = int(next_x_pos - ball->Radius - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;
	max_x = int(next_x_pos + ball->Radius - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;

	for (i = max_y; i >= min_y; --i)//y
	{
		for (j = min_x; j <= max_x; ++j)//x
		{
			if (Current_Level[i][j] == 0)
				continue;

			Current_Brick_Top_Y = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
			Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;

			Current_Brick_Left_X = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

			if (Is_Horizontal_Hit_First(next_x_pos, next_y_pos))
			{
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i);
					return true;
				}
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i);
					return true;
				}
			}
			else
			{
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i);
					return true;
				}
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i);
					return true;
				}
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
bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball)
{
	if (ball->Is_Moving_Left())//reflection from left_brick to left
	{
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Left_X, ball->Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x > 0 and Current_Level[level_y][level_x - 1] == 0)
			{
				ball->Reflect(false);//from vertical
				return true;
			}
			else
				return false;
		}
	}
	else//reflection from right_brick to right
	{
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Right_X, ball->Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x < Level_Width - 1 and Current_Level[level_y][level_x + 1] == 0)
			{
				ball->Reflect(false);//from vertical
				return true;
			}
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball)
{
	double direction = ball->Get_Direction();

	//reflection from low_brick
	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Low_Y, ball->Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y < Level_Height - 1 and Current_Level[level_y + 1][level_x] == 0)
			{
				ball->Reflect(true);//from horizontal
				return true;
			}
			else
				return false;
		}
	}
	else
	{
		//reflection from top_brick
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Top_Y, ball->Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y > 0 and Current_Level[level_y - 1][level_x] == 0)
			{
				ball->Reflect(true);//from horizontal
				return true;
			}
			else
				return false;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Act()
{
	int i;

	for (i = 0; i < Max_Active_Bricks_Count; i++)
	{
		if (Active_Bricks[i])
		{
			Active_Bricks[i]->Act();
			if (Active_Bricks[i]->Is_Finished())
			{
				delete Active_Bricks[i];
				--Active_Bricks_Count;
				Active_Bricks[i] = NULL;
			}
		}
	}

	for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
	{
		if (Falling_Letters[i])
		{
			Falling_Letters[i]->Act();
			if (!Falling_Letters[i]->Is_Finished())
			{
				delete Falling_Letters[i];
				--Falling_Letters_Count;
				Falling_Letters[i] = NULL;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ALevel::On_Hit(int level_x, int level_y)
{
	EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

	if (!Add_Falling_Letter(level_x, level_y, brick_type))
		Add_Active_Brick(level_x, level_y, brick_type);
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Add_Falling_Letter(int level_x, int level_y, EBrick_Type brick_type)
{
	int i;
	int letter_x, letter_y;
	ELetter_Type letter_type;
	AFalling_Letter *falling_letter;

	if (brick_type == EBT_Blue or brick_type == EBT_Red)
	{
		if (AsConfig::Rand(AsConfig::Hits_Per_Letter) == 0)
		{
			if (Falling_Letters_Count < AsConfig::Max_Falling_Letters_Count)
			{
				letter_type = ELT_O;
				for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
				{
					if (Falling_Letters[i] == 0)
					{
						letter_x = (AsConfig::Level_X_Offset + AsConfig::Cell_Width * level_x) * AsConfig::Global_Scale;
						letter_y = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * level_y) * AsConfig::Global_Scale;
						falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
						Falling_Letters[i] = falling_letter;
						++Falling_Letters_Count;
						break;
					}
				}
				return true;
			}
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Add_Active_Brick(int level_x, int level_y, EBrick_Type brick_type)
{
	int i;
	AActive_Brick* active_brick;

	switch (brick_type)
	{
	case EBT_Blue:
	case EBT_Red:
		active_brick = new AActive_Brick(brick_type, level_x, level_y);
		break;

	default:
		return;
	}

	for (i = 0; i < Max_Active_Bricks_Count; i++)
	{
		if (Active_Bricks[i] == 0)
		{
			Active_Bricks[i] = active_brick;
			++Active_Bricks_Count;
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------