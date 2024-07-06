#include <Windows.h>
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
int AsLevel::Active_Bricks_Count = 0;
//------------------------------------------------------------------------------------------------------------
char AsLevel::Current_Level[Level_Height][Level_Width];
//------------------------------------------------------------------------------------------------------------
char AsLevel::Level_01[AsLevel::Level_Height][AsLevel::Level_Width] = {
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//1
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//2
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//3
		2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2,//4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//6
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//7
		//2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//8
		//3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, - Unbreakable_Bricks
		//4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, - Multihits_Bricks
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8,// - Parachute_Bricks

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//12
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//13
};
//------------------------------------------------------------------------------------------------------------
char AsLevel::Test_Level[Level_Height][Level_Width] = {
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
AsLevel::AsLevel()
	: Level_Rect{}, Active_Brick(EBT_Blue, 0, 0), Current_Brick_Left_X(0), Current_Brick_Right_X(0), Current_Brick_Top_Y(0), Current_Brick_Low_Y(0), Active_Bricks{},
	Parachute_Color(AsConfig::Red_Color, AsConfig::Global_Scale, AsConfig::Blue_Color)
{}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
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
void AsLevel::Set_Current_Level(char level[Level_Height][Level_Width])
{
	memcpy(Current_Level, level, sizeof(Current_Level));
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT intersection_rect, brick_rect;
	
	//Test for letters
	/*AFalling_Letter falling_letter(EBT_Blue, ELT_Plus, 8 * AsConfig::Global_Scale, 150 * AsConfig::Global_Scale);
	falling_letter.Test_Draw_All_Steps(hdc);*/
	
	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{
		for (i = 0; i < Level_Height; ++i)
			for (j = 0; j < Level_Width; ++j)
			{
				brick_rect.left = (AsConfig::Level_X_Offset + j * AsConfig::Cell_Width) * AsConfig::Global_Scale;
				brick_rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * i) * AsConfig::Global_Scale;
				brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
				brick_rect.bottom = brick_rect.top + AsConfig::Global_Scale * AsConfig::Brick_Height;

				if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
						Draw_Brick(hdc, brick_rect, static_cast<EBrick_Type>(Current_Level[i][j]));
			}
	}

	Drow_Objects(hdc, paint_area, (AGraphics_Object **)&Active_Bricks, Max_Active_Bricks_Count);

	Drow_Objects(hdc, paint_area, (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Drow_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_counter)
{
	int i;
	for (i = 0; i < objects_max_counter; i++)
	{
		if (objects_array[i])
			objects_array[i]->Draw(hdc, paint_area);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter)
{
	AFalling_Letter *current_letter;

	if (Falling_Letters_Count == 0)
		return false;

	if (index < 0 or index >= AsConfig::Max_Falling_Letters_Count)
		return false;

	while (index < AsConfig::Max_Falling_Letters_Count)
	{
		current_letter = Falling_Letters[index++];

		if (current_letter)
		{
			*falling_letter = current_letter;
			return true;
		}
	}
	
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{
	switch (brick_type)
	{
	case EBT_None:
	case EBT_Blue:
	case EBT_Red:
		AActive_Brick_Red_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

	case EBT_Unbreakable:
		AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
		break;

	case EBT_Multihit_1:
	case EBT_Multihit_2:
	case EBT_Multihit_3:
	case EBT_Multihit_4:
		AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

	case EBT_Parachute:
		Draw_Parachute_In_Level(hdc, brick_rect);
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
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
					On_Hit(j, i, ball);
					return true;
				}
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball);
					return true;
				}
			}
			else
			{
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball);
					return true;
				}
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball);
					return true;
				}
			}
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Is_Horizontal_Hit_First(double next_x_pos, double next_y_pos)
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
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball)
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
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball)
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
void AsLevel::Act()
{
	Act_Objects((AGraphics_Object **)&Active_Bricks, Max_Active_Bricks_Count, Active_Bricks_Count);

	Act_Objects((AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count, Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act_Objects(AGraphics_Object **objects_array, const int objects_max_count, int &object_count)
{
	int i;

	for (i = 0; i < objects_max_count; i++)
	{
		if (objects_array[i])
		{
			objects_array[i]->Act();
			if (objects_array[i]->Is_Finished())
			{
				delete objects_array[i];
				--object_count;
				objects_array[i] = NULL;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::On_Hit(int level_x, int level_y, ABall *ball)
{
	EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

	if (brick_type == EBT_Parachute)
	{
		ball->Set_On_Parachute(level_x, level_y);
		Current_Level[level_y][level_x] = EBT_None;
	}
	else if (Add_Falling_Letter(level_x, level_y, brick_type))
		Current_Level[level_y][level_x] = EBT_None;
	else
		Add_Active_Brick(level_x, level_y, brick_type);

	Redraw_Brick(level_x, level_y);
	

}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Redraw_Brick(int level_x, int level_y)
{
	RECT brick_rect;

	brick_rect.left = (AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	brick_rect.top = (AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	InvalidateRect(AsConfig::Hwnd, &brick_rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int level_x, int level_y, EBrick_Type brick_type)
{
	int i;
	int letter_x, letter_y;
	ELetter_Type letter_type;
	AFalling_Letter *falling_letter;

	if (! (brick_type == EBT_Blue or brick_type == EBT_Red) )
		return false;

	if (AsConfig::Rand(AsConfig::Hits_Per_Letter) != 0)
		return false;
		
	if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_Count)
		return 0;

	for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
	{
		if (Falling_Letters[i] == 0)
		{
			letter_x = (AsConfig::Level_X_Offset + AsConfig::Cell_Width * level_x) * AsConfig::Global_Scale;
			letter_y = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * level_y) * AsConfig::Global_Scale;
			letter_type = AFalling_Letter::Get_Random_Letter_Type();

			falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
			Falling_Letters[i] = falling_letter;
			++Falling_Letters_Count;
			return true;
		}
	}
	
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick(int level_x, int level_y, EBrick_Type brick_type)
{
	int i;
	AActive_Brick* active_brick = 0;

	switch (brick_type)
	{
	case EBT_Blue:
	case EBT_Red:
		active_brick = new AActive_Brick_Red_Blue(brick_type, level_x, level_y);
		Current_Level[level_y][level_x] = EBT_None;
		break;

	case EBT_Unbreakable:
		active_brick = new AActive_Brick_Unbreakable(brick_type, level_x, level_y);
		break;

	case EBT_Multihit_1:
		active_brick = new AActive_Brick_Multihit(brick_type, level_x, level_y);
		Current_Level[level_y][level_x] = EBT_None;
		break;

	case EBT_Multihit_2:
	case EBT_Multihit_3:
	case EBT_Multihit_4:
		Current_Level[level_y][level_x] = brick_type - 1;
		break;

	default:
		AsConfig::Throw();
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
void AsLevel::Draw_Parachute_In_Level(HDC hdc, RECT brick_rect)
{
	int i, width, height;
	RECT top_rect, bottom_rect;

	Parachute_Color.Select(hdc);
	top_rect.left = brick_rect.left + 1;
	top_rect.top = brick_rect.top + 1;
	
	height = 3 * AsConfig::Global_Scale;

	for (i = 0; i < 3; i++)
	{
		if (i % 2 == 1)
		{
			width = 6 * AsConfig::Global_Scale;
			top_rect.right = top_rect.left + width + 1;
		}
		else
		{
			width = 4 * AsConfig::Global_Scale;
			top_rect.right = top_rect.left + width + 1;
			top_rect.bottom = top_rect.top + height + 1;
		}

		AsConfig::Round_Rect(hdc, top_rect);
		bottom_rect = top_rect;
		bottom_rect.top += height;
		bottom_rect.bottom += height;
		AsConfig::Round_Rect(hdc, bottom_rect);
		top_rect.left += width;
	}
}
//------------------------------------------------------------------------------------------------------------
