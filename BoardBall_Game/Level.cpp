#include <Windows.h>
#include "Level.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
int AsLevel::Active_Bricks_Count = 0;
AsLevel *AsLevel::Level = 0;
//------------------------------------------------------------------------------------------------------------
char AsLevel::Level_01[AsLevel::Level_Height][AsLevel::Level_Width] = {
	//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//1
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//2
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//3
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,//5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,//6
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,//9 2, 2, 0,10,10, 0,//7
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,//8
		//3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,// -  Unbreakable_Bricks 
		//4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7,// - Multihits_Bricks
		//8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,// - Parachute_Bricks
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
AsLevel::~AsLevel()
{
	Clear_All_Activity();

	delete[] Teleport_Bricks_Pos;
	Teleport_Bricks_Pos = 0;
}
//------------------------------------------------------------------------------------------------------------
AsLevel::AsLevel()
	: Teleport_Bricks_Count(0),Teleport_Bricks_Pos(0), Level_Rect{}, Active_Brick(EBrick_Type::Blue, 0, 0), 
	Current_Brick_Left_X(0), Current_Brick_Right_X(0), Current_Brick_Top_Y(0), Current_Brick_Low_Y(0), 
	Active_Bricks{}, Advertisement(0), Parachute_Color(AsConfig::Red_Color, AsConfig::Global_Scale, AsConfig::Blue_Color), 
	Should_Stop_Level(false), Current_Level{}
{
	Level = this;
}
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
	int i, j, teleport_index;
	EBrick_Type brick_type;

	memcpy(Current_Level, level, sizeof(Current_Level));

	Teleport_Bricks_Count = 0;

	for (i = 0; i < Level_Height; i++)
	{
		for (j = 0; j < Level_Width; j++)
		{
			brick_type = static_cast<EBrick_Type>(Current_Level[i][j]);

			if (brick_type == EBrick_Type::Teleport)
				++Teleport_Bricks_Count;
		}
	}

	delete[] Teleport_Bricks_Pos;
	Teleport_Bricks_Pos = 0;

	if (Teleport_Bricks_Count != 0)
	{
		if (Teleport_Bricks_Count == 1)
			AsConfig::Throw();

		Teleport_Bricks_Pos = new SPoint[Teleport_Bricks_Count];
		teleport_index = 0;

		for (i = 0; i < Level_Height; i++)
		{
			for (j = 0; j < Level_Width; j++)
			{
				brick_type = static_cast<EBrick_Type>(Current_Level[i][j]);

				if (brick_type == EBrick_Type::Teleport)
					Teleport_Bricks_Pos[teleport_index++] = SPoint{j, i};
			}
		}
	}

	Advertisement = new AAdvertisement(9, 6, 2, 3);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_counter)
{
	int i;

	for (i = 0; i < objects_max_counter; i++)
	{
		if (objects_array[i])
			objects_array[i]->Clear_Prev_Animation(hdc, paint_area);
	}
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
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, int level_x, int level_y)
{
	EBrick_Type brick_type = static_cast<EBrick_Type>(Current_Level[level_y][level_x]);

	switch (brick_type)
	{
	case EBrick_Type::None:
		if (Advertisement != 0 and Advertisement->Has_Brick_At_Position(level_x, level_y) )
			break;

	case EBrick_Type::Blue:
	case EBrick_Type::Red:
		AActive_Brick_Red_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

	case EBrick_Type::Unbreakable:
		AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Multihit_1:
	case EBrick_Type::Multihit_2:
	case EBrick_Type::Multihit_3:
	case EBrick_Type::Multihit_4:
		AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
		break;

	case EBrick_Type::Parachute:
		Draw_Parachute_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Teleport:
		AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect, 0);
		break;

	case EBrick_Type::Ad:
		AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
		break;

	case EBrick_Type::Invisible:
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{
	int i, j;  
	double direction = ball->Get_Direction();
	int min_y, max_y, min_x, max_x;

	if (next_y_pos + AsConfig::Ball_Radius > AsConfig::Border_Y_Offset + (Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Cell_Height)
		return false;

	min_y = int(next_y_pos - AsConfig::Ball_Radius - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;
	max_y = int(next_y_pos + AsConfig::Ball_Radius - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;

	min_x = int(next_x_pos - AsConfig::Ball_Radius - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;
	max_x = int(next_x_pos + AsConfig::Ball_Radius - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;

	if (min_y < 0)
		min_y = 0;

	if (max_y >= Level_Height)
		max_y = Level_Height - 1;

	if (min_x < 0)
		min_x = 0;

	if (max_x >= Level_Width)
		max_x = Level_Width - 1;

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
					On_Hit(j, i, ball, false);
					return true;
				}
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball, true);
					return true;
				}
			}
			else
			{
				if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball, true);
					return true;
				}
				if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
				{
					On_Hit(j, i, ball, false);
					return true;
				}
			}
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos)
{
	int brick_x, brick_y;

	brick_x = int(next_x_pos - AsConfig::Level_X_Offset) / AsConfig::Cell_Width;
	brick_y = int(next_y_pos - AsConfig::Level_Y_Offset) / AsConfig::Cell_Height;

	if (brick_x >= Level_Width or brick_x < 0)
		return false;

	if (brick_y >= Level_Height or brick_y < 0)
		return false;

	if ((EBrick_Type)Current_Level[brick_y][brick_x] == EBrick_Type::None)
		return false;


	On_Hit(brick_x, brick_y, 0, true);
	return true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Begin_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Finish_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Shift_Per_Step(double max_speed)
{
}
//------------------------------------------------------------------------------------------------------------
double AsLevel::Get_Speed()
{
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act()
{
	Act_Objects((AGraphics_Object **)&Active_Bricks, Max_Active_Bricks_Count, Active_Bricks_Count);

	Act_Objects((AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count, Falling_Letters_Count);

	if (Advertisement != 0)
		Advertisement->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT intersection_rect{}, brick_rect{};

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
					Draw_Brick(hdc, brick_rect, j, i);

			}

		Drow_Objects(hdc, paint_area, (AGraphics_Object **)&Active_Bricks, Max_Active_Bricks_Count);
	}

	Drow_Objects(hdc, paint_area, (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	Clear_Objects(hdc, paint_area, (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);

	if (Advertisement != 0)
		Advertisement->Draw(hdc, paint_area);

	if (Should_Stop_Level)
	{
		Clear_All_Activity();
		Should_Stop_Level = false;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Is_Finished()
{
	return false;// code stub
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Has_Brick_At_Pos(int level_x_pos, int level_y_pos)
{
	EBrick_Type brick_type;

	if (level_x_pos < 0 || level_x_pos >= Level_Width)
		return false;

	if(level_y_pos < 0 || level_y_pos >= Level_Height)
		return false;

	brick_type = (EBrick_Type)Level->Current_Level[level_y_pos][level_x_pos];

	if (brick_type == EBrick_Type::None)
		return false;
	else
		return true;
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Has_Brick_At_Rect(RECT rect)
{
	int x, y;
	const int &scale = AsConfig::Global_Scale;

	int min_level_x, max_level_x;
	int min_level_y, max_level_y;

	int cell_width = AsConfig::Cell_Width * scale;
	int cell_height = AsConfig::Cell_Height * scale;

	int x_offset = AsConfig::Level_X_Offset * scale;
	int y_offset = AsConfig::Level_Y_Offset * scale;

	min_level_x = (rect.left - x_offset) / cell_width;

	max_level_x = (rect.right - x_offset) / cell_width;

	min_level_y = (rect.top - y_offset) / cell_height;

	max_level_y = (rect.bottom - y_offset) / cell_height;

	for (y = min_level_y; y <= max_level_y; y++)
		for (x = min_level_x; x <= max_level_x; x++)
			if (Has_Brick_At_Pos(x, y) )
				return true;

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
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object* ball)
{
	if (ball->Is_Moving_Left())//reflection from left_brick to left
	{
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Left_X, AsConfig::Ball_Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x > 0 and Current_Level[level_y][level_x - 1] == 0)
			{
				if ((EBrick_Type)Current_Level[level_y][level_x] != EBrick_Type::Teleport)
					ball->Reflect(false);//from vertical

				return true;
			}
			else
				return false;
		}
	}
	else//reflection from right_brick to right
	{
		if (Hit_Circle_On_Line(next_y_pos, next_x_pos - Current_Brick_Right_X, AsConfig::Ball_Radius, Current_Brick_Top_Y, Current_Brick_Low_Y))
		{
			if (level_x < Level_Width - 1 and Current_Level[level_y][level_x + 1] == 0)
			{
				if ((EBrick_Type)Current_Level[level_y][level_x] != EBrick_Type::Teleport)
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
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object* ball)
{
	double direction = ball->Get_Direction();

	//reflection from low_brick
	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Low_Y, AsConfig::Ball_Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y < Level_Height - 1 and Current_Level[level_y + 1][level_x] == 0)
			{
				if ((EBrick_Type)Current_Level[level_y][level_x] != EBrick_Type::Teleport)
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
		if (Hit_Circle_On_Line(next_x_pos, next_y_pos - Current_Brick_Top_Y, AsConfig::Ball_Radius, Current_Brick_Left_X, Current_Brick_Right_X))
		{
			if (level_y > 0 and Current_Level[level_y - 1][level_x] == 0)
			{
				if ((EBrick_Type)Current_Level[level_y][level_x] != EBrick_Type::Teleport)
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
void AsLevel::On_Hit(int level_x, int level_y, ABall_Object *ball, bool vertical_hit)
{
	EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

	if (brick_type == EBrick_Type::Parachute and ball == 0)
	{
		brick_type = EBrick_Type::Red;
		Current_Level[level_y][level_x] = (char)brick_type;
	}

	if (brick_type == EBrick_Type::Parachute)
	{
		ball->Set_On_Parachute(level_x, level_y);
		Current_Level[level_y][level_x] = (char)EBrick_Type::None;
	}
	else if (Add_Falling_Letter(level_x, level_y, brick_type))
		Current_Level[level_y][level_x] = (char)EBrick_Type::None;
	else
		Create_Active_Brick(level_x, level_y, brick_type, ball, vertical_hit);

	Redraw_Brick(level_x, level_y);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Redraw_Brick(int level_x, int level_y)
{
	RECT brick_rect{};

	brick_rect.left = (AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	brick_rect.top = (AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(brick_rect);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int level_x, int level_y, EBrick_Type brick_type)
{
	int i;
	int letter_x, letter_y;
	ELetter_Type letter_type;
	AFalling_Letter *falling_letter;

	if (! (brick_type == EBrick_Type::Blue or brick_type == EBrick_Type::Red) )
		return false;

	if (AsTools::Rand(AsConfig::Hits_Per_Letter) != 0)
		return false;
		
	if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_Count)
		return 0;

	for (i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
	{
		if (Falling_Letters[i] == 0)
		{
			letter_x = (AsConfig::Level_X_Offset + AsConfig::Cell_Width * level_x) * AsConfig::Global_Scale;
			letter_y = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * level_y) * AsConfig::Global_Scale;
			letter_type = ELetter_Type::L;//AFalling_Letter::Get_Random_Letter_Type();

			switch (AsTools::Rand(3) )
			{
			case 0:
				letter_type = ELetter_Type::L;
				break;

			case 1:
				letter_type = ELetter_Type::W;
				break;

			case 2:
				letter_type = ELetter_Type::K;
				break;

			case 3:
				letter_type = ELetter_Type::T;
				break;
			}

			falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
			Falling_Letters[i] = falling_letter;
			++Falling_Letters_Count;
			return true;
		}
	}
	
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Create_Active_Brick(int level_x, int level_y, EBrick_Type brick_type, ABall_Object *ball, bool vertical_hit)
{
	AActive_Brick* active_brick = 0;

	switch (brick_type)
	{
	case EBrick_Type::None:
		return;

	case EBrick_Type::Blue:
	case EBrick_Type::Red:
		active_brick = new AActive_Brick_Red_Blue(brick_type, level_x, level_y);
		Current_Level[level_y][level_x] = (char)EBrick_Type::None;
		break;

	case EBrick_Type::Unbreakable:
		active_brick = new AActive_Brick_Unbreakable(brick_type, level_x, level_y);
		break;

	case EBrick_Type::Multihit_1:
		active_brick = new AActive_Brick_Multihit(brick_type, level_x, level_y);
		Current_Level[level_y][level_x] = (char)EBrick_Type::None;
		break;

	case EBrick_Type::Multihit_2:
	case EBrick_Type::Multihit_3:
	case EBrick_Type::Multihit_4:
		Current_Level[level_y][level_x] = (char)brick_type - 1;
		break;

	case EBrick_Type::Teleport:
		if (ball != 0)
			Add_Active_Brick_Teleport(level_x, level_y, ball, vertical_hit);
		return;

	case EBrick_Type::Ad:
		active_brick = new AActive_Brick_Ad(brick_type, level_x, level_y, Advertisement);
		Current_Level[level_y][level_x] = (char)EBrick_Type::Invisible;
		break;

	case EBrick_Type::Invisible:
		break;

	default:
		AsConfig::Throw();
	}

	if (active_brick != 0)
		Add_New_Active_Brick(active_brick);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick_Teleport(int level_x, int level_y, ABall_Object *ball, bool vertical_hit)
{
	int i;
	bool got_direction;
	double prev_teleport_x_pos, prev_teleport_y_pos;
	double curr_ball_x_pos, curr_ball_y_pos;
	int dest_brick_x, dest_brick_y;
	EDirection_Type direction;

	AActive_Brick_Teleport *destination_teleport = 0;
	AActive_Brick_Teleport *source_teleport = 0;

	ball->Get_Center(prev_teleport_x_pos, prev_teleport_y_pos);

	destination_teleport = Select_Destination_Teleport(level_x, level_y);
	source_teleport = new AActive_Brick_Teleport(level_x, level_y, ball, destination_teleport);

	destination_teleport->Get_Level_Pos(dest_brick_x, dest_brick_y);

	ball->Get_Center(curr_ball_x_pos, curr_ball_y_pos);

	if (vertical_hit)
	{
		if (curr_ball_y_pos < prev_teleport_y_pos)
			direction = EDirection_Type::Up;
		else
			direction = EDirection_Type::Down;
	}
	else
	{
		if (curr_ball_x_pos < prev_teleport_y_pos)
			direction = EDirection_Type::Left;
		else
			direction = EDirection_Type::Down;
	}

	got_direction = false;

	for (i = 0; i < 4; i++)
	{
		switch (direction)
		{
		case EDirection_Type::Left:
			if (dest_brick_x > 0 and (EBrick_Type)Current_Level[dest_brick_y][dest_brick_x - 1] == EBrick_Type::None)
				got_direction = true;
			break;
		
		case EDirection_Type::Up:
			if (dest_brick_y > 0 and (EBrick_Type)Current_Level[dest_brick_y - 1][dest_brick_x] == EBrick_Type::None)
				got_direction = true;
			break;
		

		case EDirection_Type::Right:
			if (dest_brick_x < Level_Width - 1 and (EBrick_Type)Current_Level[dest_brick_y][dest_brick_x + 1] == EBrick_Type::None)
				got_direction = true;
			break;
		

		case EDirection_Type::Down:
			if (dest_brick_y < Level_Height - 1 and (EBrick_Type)Current_Level[dest_brick_y + 1][dest_brick_x] == EBrick_Type::None)
				got_direction = true;
			break;

		default:
			AsConfig::Throw();

		}

		if (got_direction)
			break;

		direction = EDirection_Type( (char)direction - 1);
		
		ball->Set_Direction(ball->Get_Direction() + M_PI_2);

		if (direction < (EDirection_Type)0)
			direction = EDirection_Type::Down;

	}

	Add_New_Active_Brick(source_teleport);

	if (destination_teleport != 0)
	{
		Add_New_Active_Brick(destination_teleport);
		destination_teleport->Release_Direction = direction;
	}
}
//------------------------------------------------------------------------------------------------------------ 
void AsLevel::Add_New_Active_Brick(AActive_Brick *active_brick)
{
	int i;

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
AActive_Brick_Teleport *AsLevel::Select_Destination_Teleport(int source_x, int source_y)
{
	int dest_index;
	AActive_Brick_Teleport *destination_teleport;

	if (Teleport_Bricks_Count < 2)
	{
		AsConfig::Throw();
		return 0;
	}

	dest_index = AsTools::Rand(Teleport_Bricks_Count);
	
	if (source_x == Teleport_Bricks_Pos[dest_index].X and source_y == Teleport_Bricks_Pos[dest_index].Y)
		++dest_index;

	if (dest_index >= Teleport_Bricks_Count)
		dest_index = 0;

	destination_teleport = new AActive_Brick_Teleport(Teleport_Bricks_Pos[dest_index].X, Teleport_Bricks_Pos[dest_index].Y, 0, 0);

	return destination_teleport;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Parachute_In_Level(HDC hdc, RECT brick_rect)
{
	int i, width, height;
	RECT top_rect{}, bottom_rect{};

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

		AsTools::Round_Rect(hdc, top_rect);
		bottom_rect = top_rect;
		bottom_rect.top += height;
		bottom_rect.bottom += height;
		AsTools::Round_Rect(hdc, bottom_rect);
		top_rect.left += width;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Stop()
{
	Should_Stop_Level = true;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Clear_All_Activity()
{
	Delete_Objects( (AGraphics_Object **)&Active_Bricks, Max_Active_Bricks_Count, Active_Bricks_Count);

	Delete_Objects( (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count, Falling_Letters_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Delete_Objects(AGraphics_Object **objects_array, const int objects_max_count, int &object_count)
{
	int i;

	for (i = 0; i < objects_max_count; i++)
	{
		if (objects_array[i])
		{
			delete objects_array[i];
			objects_array[i] = 0;
		}
		object_count = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
