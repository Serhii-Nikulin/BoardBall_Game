#include "Monster.h"
#include "Gate.h"

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
	double next_step;

	next_step = Speed * AsConfig::Moving_Step_Size / max_speed;

	X_Pos += next_step * cos(Direction);
	Y_Pos -= next_step * sin(Direction);

	if (Monster_State == EMonster_State::Alive)
	{
		if (X_Pos < (double)AsConfig::Level_X_Offset)
		{
			X_Pos = (double)AsConfig::Level_X_Offset;
			Direction += M_PI;
		}

		if (Y_Pos < (double)AsConfig::Level_Y_Offset)
		{
			Y_Pos = (double)AsConfig::Level_Y_Offset;
			Direction += M_PI;
		}

		if (X_Pos > (double)AsConfig::Max_X_Pos - Width)
		{
			X_Pos = (double)AsConfig::Max_X_Pos - Width;
			Direction += M_PI;
		}

		if (Y_Pos > (double)AsConfig::Max_Y_Pos - Height)
		{
			Y_Pos = (double)AsConfig::Max_Y_Pos - Height;
			Direction += M_PI;
		}
	}
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

	Monster_Rect.left = (int)(X_Pos * AsConfig::D_Global_Scale);
	Monster_Rect.top = (int)(Y_Pos * AsConfig::D_Global_Scale);
	Monster_Rect.right = Monster_Rect.left + Width * AsConfig::Global_Scale;
	Monster_Rect.bottom = Monster_Rect.top + Height * AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(Prev_Monster_Rect);
	AsTools::Invalidate_Rect(Monster_Rect);
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

	for (i = 0; i < Explosive_Balls_Count; i++)
		Explosive_Balls[i].Draw(hdc, paint_area);
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
bool AMonster::Is_Active()
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
