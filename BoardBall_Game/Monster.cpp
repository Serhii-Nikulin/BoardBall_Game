#include "Monster.h"
#include "Gate.h"
#include "Level.h"

//AMonster
//------------------------------------------------------------------------------------------------------------
const double AMonster::Blink_Timeouts[AMonster::Blink_Stages_Count] = { 0.5, 0.5, 1.0, 0.5, 
0.5, 0.5, 1.2};
const EEye_State AMonster::Blink_States[AMonster::Blink_Stages_Count] = {EEye_State::Closed, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
	: Monster_State(EMonster_State::Missing), Monster_Rect{}, Prev_Monster_Rect{}, X_Pos(0.0), Y_Pos(0.0), Speed(0.0), Direction(0.0), Cornea_Height(0), Eye_State(EEye_State::Closed), Blink_Ticks{}, Start_Blink_Timeout(0), Total_Animation_Timeout(0), Direction_Switch_Tick(0), Alive_Timer_Tick(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	// stub
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	if (! Is_Active() )
		return;

	Redraw_Monster();
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Shift_Per_Step(double max_speed)
{
	int i;
	double next_step;
	double next_x_pos, next_y_pos;
	RECT monster_rect;
	double angle_correction = M_PI / 8.0;
	int max_corrections_count = int(M_PI * 2.0 / angle_correction);
	double origin_direction = Direction;
	bool got_hit = false;
	double max_y_pos = (double)AsConfig::Max_Y_Pos - 1.0 - Height;

	if (! (Monster_State == EMonster_State::Missing or Monster_State == EMonster_State::Alive) )
		return;

	next_step = Speed * AsConfig::Moving_Step_Size / max_speed;

	for (i = 0; i < max_corrections_count; i++)
	{
		next_x_pos = X_Pos + next_step * cos(Direction);
		next_y_pos = Y_Pos - next_step * sin(Direction);

		Update_Rect( (int)next_x_pos, (int)next_y_pos, monster_rect);

		if (AsLevel::Has_Brick_At_Rect(monster_rect) )
			Direction += angle_correction;
		else
		{
			got_hit = true;
			break;
		}
	}

	if (! got_hit)
	{
		Direction = origin_direction - M_PI;
		return;
	}

	if (Monster_State == EMonster_State::Alive)
	{
		if (next_x_pos < (double)AsConfig::Level_X_Offset)
		{
			next_x_pos = (double)AsConfig::Level_X_Offset;
			Direction += M_PI;
		}

		if (next_y_pos < (double)AsConfig::Level_Y_Offset)
		{
			next_y_pos = (double)AsConfig::Level_Y_Offset;
			Direction += M_PI;
		}

		if (next_x_pos > (double)AsConfig::Max_X_Pos - Width)
		{
			next_x_pos = (double)AsConfig::Max_X_Pos - Width;
			Direction += M_PI;
		}

		if (next_y_pos > max_y_pos)
		{
			next_y_pos = max_y_pos;
			Direction += M_PI;
		}
	}

	X_Pos = next_x_pos;
	Y_Pos = next_y_pos;
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return Speed;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	switch(Monster_State)
	{
	case EMonster_State::Missing:
		return;

	case EMonster_State::Emitting:
		if (AsConfig::Current_Timer_Tick >= Alive_Timer_Tick)
			Monster_State = EMonster_State::Alive;
		break;

	case EMonster_State::Alive:
		Act_Alive();
		break;

	case EMonster_State::Destroying:
		Act_Destroying();
		break;

	default:
		AsConfig::Throw();
	}

	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Alive()
{
	int i;
	int current_tick_offset, prev_tick;
	double ratio;
	double direction_delta;

	current_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blink_Timeout) % Total_Animation_Timeout;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		if (current_tick_offset < Blink_Ticks[i])
		{
			Eye_State = Blink_States[i];
			break;
		}	
	}

	if (i == 0)
		prev_tick = 0;
	else
		prev_tick = Blink_Ticks[i - 1];

	ratio = ( (double)current_tick_offset - prev_tick) / (Blink_Ticks[i] - prev_tick);

	switch (Eye_State)
	{
	case EEye_State::Closed:
		Cornea_Height = 0.0;
		break;

	case EEye_State::Opening:
		Cornea_Height = ratio * Max_Cornea_Height;
		break;

	case EEye_State::Staring:
		Cornea_Height = Max_Cornea_Height;
		break;

	case EEye_State::Closing:
		Cornea_Height = (1.0 - ratio) * Max_Cornea_Height;
		break;

	default:
		AsConfig::Throw();
	}

	if (AsConfig::Current_Timer_Tick > Direction_Switch_Tick)
	{
		direction_delta = (45 - AsTools::Rand(90) ) * M_PI / 180.0;
		Direction += direction_delta;
		Direction_Switch_Tick += AsConfig::FPS + AsTools::Rand(AsConfig::FPS * 2);
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Destroying()
{
	int i;

	for (i = 0; i < Explosive_Balls_Count; i++)
		Explosive_Balls[i].Act();
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Redraw_Monster()
{
	Prev_Monster_Rect = Monster_Rect;

	Update_Rect( (int)X_Pos, (int)Y_Pos, Monster_Rect);

	AsTools::Invalidate_Rect(Prev_Monster_Rect);
	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Update_Rect(int x_pos, int y_pos, RECT &rect)
{
	rect.left = (int)(x_pos * AsConfig::D_Global_Scale);
	rect.top = (int)(y_pos * AsConfig::D_Global_Scale);
	rect.right = rect.left + Width * AsConfig::Global_Scale;
	rect.bottom = rect.top + Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &Monster_Rect, &paint_area) )
		return;

	switch (Monster_State)
	{
	case EMonster_State::Missing:
		break;

	case EMonster_State::Emitting:
	case EMonster_State::Alive:
		Draw_Alive(hdc);
		break;

	case EMonster_State::Destroying:
		Draw_Destroying(hdc, paint_area);
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Alive(HDC hdc)
{
	HRGN region;

	RECT rect, cornea_rect;
	const int scale = AsConfig::Global_Scale;
	const int half_scale = scale / 2;

	rect = Monster_Rect;
	rect.right += 1;
	rect.bottom += 1;

	region = CreateEllipticRgnIndirect(&rect);

	SelectClipRgn(hdc, region);
	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Monster_Dark_Red_Color);

	rect = Monster_Rect;

	rect.left -= 2 * scale;
	rect.right -= 2 * scale;
	rect.top -= 3 * scale;
	rect.bottom -= 3 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	if (Eye_State == EEye_State::Closed)
		return;

	//Draw cornea
	cornea_rect = Monster_Rect;
	cornea_rect.left += 1 * scale + half_scale;
	cornea_rect.right -= 1 * scale + half_scale;
	cornea_rect.top += int( (Height - Cornea_Height) / 2.0 * AsConfig::D_Global_Scale);
	cornea_rect.bottom -= int( (Height - Cornea_Height) / 2.0 * AsConfig::D_Global_Scale);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	region = CreateEllipticRgnIndirect(&cornea_rect);
	SelectClipRgn(hdc, region);

	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Monster_Cornea_Color);

	//Draw iris
	rect = Monster_Rect;
	rect.left += 4 * scale + half_scale;
	rect.right -= 4 * scale + half_scale;
	rect.top += 5 * scale;
	rect.bottom -= 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Monster_Iris_Color);

	//Draw pupil
	rect = Monster_Rect;
	rect.left += 7 * scale;
	rect.right = rect.left + 2 * scale;
	rect.top += 6 * scale + half_scale;
	rect.bottom = rect.top + 3 * scale + half_scale;

	AsTools::Ellipse(hdc, rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	AsConfig::BG_Outline_Color.Select(hdc);
	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right - 1, cornea_rect.bottom - 1, 0, 0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Destroying(HDC hdc, RECT &paint_area)
{
	int i;
	bool destroying_is_finished = true;

	for (i = 0; i < Explosive_Balls_Count; i++)
	{
		Explosive_Balls[i].Draw(hdc, paint_area);
		destroying_is_finished &= Explosive_Balls[i].Is_Finished();
	}

	if (destroying_is_finished)
		Monster_State = EMonster_State::Missing;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Monster_Rect) )
		return;

	AsTools::Ellipse(hdc, Prev_Monster_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball)
{
	if (! (Monster_State == EMonster_State::Emitting or Monster_State == EMonster_State::Alive) )
		return false;

	double radius = (double)Width / 2.0;

	if (! AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, X_Pos + radius, Y_Pos + radius, radius, ball) )
		return false;

	Destroy();
	
	return true;
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos)
{
	if (! (Monster_State == EMonster_State::Emitting or Monster_State == EMonster_State::Alive) )
		return false;

	if (next_x_pos >= X_Pos and next_x_pos <= X_Pos + Width)
		if (next_y_pos >= Y_Pos and next_y_pos <= Y_Pos + Height)
		{
			Destroy();
			return true;
		}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Check_Hit(RECT &rect)
{
	RECT intersection_rect;
	RECT monster_rect;

	if (! (Monster_State == EMonster_State::Emitting or Monster_State == EMonster_State::Alive) )
		return false;

	Update_Rect( (int)X_Pos, (int)Y_Pos, monster_rect);

	if (IntersectRect(&intersection_rect, &rect, &monster_rect) )
	{
		Destroy();
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Activate(int x_pos, int y_pos, bool moving_right)
{
	int i;
	const int scale = AsConfig::Global_Scale;
	double current_timeout = 0.0;
	int emitting_time_offset;

	Monster_State = EMonster_State::Emitting;
	X_Pos = x_pos;
	Y_Pos = y_pos;
	Speed = AsConfig::Moving_Step_Size * (1 + AsTools::Rand(3) );

	emitting_time_offset = (int)(AGate::Width / Speed);
	Alive_Timer_Tick = AsConfig::Current_Timer_Tick + emitting_time_offset;

	if (moving_right)
		Direction = 0.0;
	else
	{
		Direction = M_PI;
		X_Pos = X_Pos - Width + AGate::Width;
	}

	Start_Blink_Timeout = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		current_timeout += Blink_Timeouts[i];
		Blink_Ticks[i] = (int)(current_timeout * AsConfig::FPS);
	}

	Total_Animation_Timeout = Blink_Ticks[i - 1];

	Redraw_Monster();
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Active() const
{
	if (Monster_State == EMonster_State::Missing)
		return false;
	else
		return true;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Destroy()
{
	int i;
	const int& scale = AsConfig::Global_Scale;
	int half_width = Width * scale / 2;
	int half_height = Height * scale / 2;

	int x_pos = (int)(X_Pos * scale + half_width);
	int y_pos = (int)(Y_Pos * scale + half_height);
	int x_offset, y_offset;
	int size, time_offset;
	int half_size, rest_size;

	half_size = half_width;

	if (half_size > half_height)
		half_size = half_height;

	Monster_State = EMonster_State::Destroying;

	for (i = 0; i < Explosive_Balls_Count; i++)
	{
		time_offset = AsTools::Rand(AsConfig::FPS * 2);

		x_offset = AsTools::Rand(half_width) - half_width / 2;
		y_offset = AsTools::Rand(half_height) - half_height / 2;

		rest_size = half_size - (int)sqrt(x_offset * x_offset + y_offset * y_offset);
		size = AsTools::Rand(rest_size / 2) + rest_size / 2;

		if (size < scale)
			size = scale;

		Explosive_Balls[i].Explode(x_pos + x_offset, y_pos + y_offset, size * 2, AsTools::Rand(2), time_offset, 10);
	}
}
//------------------------------------------------------------------------------------------------------------
