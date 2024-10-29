#include "Monster_Set.h"

//AExplosive_Ball
AColor AExplosive_Ball::Fading_Red_Colors[Max_Fade_Step] = {};
AColor AExplosive_Ball::Fading_Blue_Colors[Max_Fade_Step] = {};
//------------------------------------------------------------------------------------------------------------
AExplosive_Ball::AExplosive_Ball()
: X_Pos(0), Y_Pos(0), Size(0.0), Max_Size(0.0), Step_Count(0), Size_Step(0.0), Explosive_Ball_Rect{}, Explosive_Ball_State(EExplosive_Ball_State::Idle), Start_Fading_Tick(0), Time_Offset(0), Start_Expanding_Tick(0), Is_Red(false)
{
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act()
{
	switch(Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
		break;

	case EExplosive_Ball_State::Charging:

		if(AsConfig::Current_Timer_Tick >= Start_Expanding_Tick)
			Explosive_Ball_State = EExplosive_Ball_State::Expanding;

		break;

	case EExplosive_Ball_State::Expanding:

		Size += Size_Step;

		if (Size >= Max_Size)
		{
			Size = Max_Size;
			Explosive_Ball_State = EExplosive_Ball_State::Fading;
			Start_Fading_Tick = AsConfig::Current_Timer_Tick;
		}
		else
			Update_Exploding_Ball_Rect();

		break;

	case EExplosive_Ball_State::Fading:
		if (AsConfig::Current_Timer_Tick > Start_Fading_Tick + Fading_Timeout)
			Explosive_Ball_State = EExplosive_Ball_State::Idle;

		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Draw(HDC hdc, RECT &paint_area)
{
	int current_timeout;
	int color_index;
	double ratio;
	const AColor *color = 0;

	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
	case EExplosive_Ball_State::Charging:
		return;

	case EExplosive_Ball_State::Expanding:

		if(Is_Red)
			color = &AsConfig::Explosion_Red_Color;
		else
			color = &AsConfig::Explosion_Blue_Color;
		
		AsTools::Ellipse(hdc, Explosive_Ball_Rect, *color);

		break;

	case EExplosive_Ball_State::Fading:
		current_timeout = AsConfig::Current_Timer_Tick - Start_Fading_Tick;

		if (current_timeout > Fading_Timeout)
			current_timeout = Fading_Timeout;

		if (current_timeout == Fading_Timeout)
			AsTools::Ellipse(hdc, Explosive_Ball_Rect, AsConfig::BG_Color);
		else
		{
			ratio = (double)current_timeout / (double)Fading_Timeout;
			color_index = (int)round( (Max_Fade_Step - 1) * ratio);

			if(Is_Red)
				color = &Fading_Red_Colors[color_index];
			else
				color = &Fading_Blue_Colors[color_index];

			AsTools::Ellipse(hdc, Explosive_Ball_Rect, *color);
		}

		break;
	}
	
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	// stub
}
//------------------------------------------------------------------------------------------------------------
bool AExplosive_Ball::Is_Finished()
{
	return false; // stub
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Explode(int x_pos, int y_pos, double max_size, bool is_red, int time_offset, int step_count)
{
	Explosive_Ball_State = EExplosive_Ball_State::Charging;

	Size = 0.0;
	X_Pos = x_pos;
	Y_Pos = y_pos;
	Max_Size = max_size;
	Step_Count = step_count;
	Size_Step = (double)Max_Size / (double)Step_Count;
	Time_Offset = time_offset;
	Start_Expanding_Tick = AsConfig::Current_Timer_Tick + time_offset;
	Is_Red = is_red;

	Update_Exploding_Ball_Rect();
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Setup_Colors()
{
	int i;

	for (i = 0; i < Max_Fade_Step; i++)
	{
		AsTools::Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Colors[i], Max_Fade_Step);
		AsTools::Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Colors[i], Max_Fade_Step);
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Update_Exploding_Ball_Rect()
{
	Explosive_Ball_Rect.left = (int)(X_Pos - (Size / 2.0) );
	Explosive_Ball_Rect.right = (int)(Explosive_Ball_Rect.left + Size);
	Explosive_Ball_Rect.top = (int)(Y_Pos - (Size / 2.0) );
	Explosive_Ball_Rect.bottom = (int)(Explosive_Ball_Rect.top + Size);

	//AsTools::Invalidate_Rect(Explosive_Ball_Rect);
}
//------------------------------------------------------------------------------------------------------------



//AMonster
//------------------------------------------------------------------------------------------------------------
const double AMonster::Blink_Timeouts[AMonster::Blink_Stages_Count] = { 0.5, 0.5, 1.0, 0.5, 
	0.5, 0.5, 1.2};
const EEye_State AMonster::Blink_States[AMonster::Blink_Stages_Count] = {EEye_State::Closed, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
	: Monster_State(EMonster_State::Missing), Monster_Rect{}, Prev_Monster_Rect{}, X_Pos(0.0), Y_Pos(0.0), Speed(0.0), Direction(0.0), Cornea_Height(0), Eye_State(EEye_State::Closed), Blink_Ticks{}, Start_Blink_Timeout(0), Total_Animation_Timeout(0)
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

	next_step = Speed * AsConfig::Moving_Step_Size / max_speed ;
	X_Pos += next_step * cos(Direction);
	Y_Pos -= next_step * sin(Direction);
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
	double scale = AsConfig::Global_Scale;

	Prev_Monster_Rect = Monster_Rect;

	Monster_Rect.left = (int)(X_Pos * scale);
	Monster_Rect.top = (int)(Y_Pos * scale);
	Monster_Rect.right = Monster_Rect.left + Width * scale;
	Monster_Rect.bottom = Monster_Rect.top + Height * scale;

	AsTools::Invalidate_Rect(Prev_Monster_Rect);
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

	if (Monster_State == EMonster_State::Alive)
		return;

	Monster_State = EMonster_State::Alive;
	X_Pos = x_pos;
	Y_Pos = y_pos;
	Speed = AsConfig::Moving_Step_Size;

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




//AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
: Border(0), Monsters{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder *border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_From_Gate(int gate_index)
{
	int i;
	int gate_x_pos, gate_y_pos;
	bool gate_is_left;

	AMonster *monster = 0;

	if (gate_index < 0 or gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (! Monsters[i].Is_Active() )
		{
			monster = &Monsters[i];
			break;
		}
	}

	if (monster == 0)
		return;

	if (gate_index % 2 == 0)
		gate_is_left = true;
	else
		gate_is_left = false;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);
	monster->Activate(gate_x_pos, gate_y_pos, gate_is_left);
	//monster->Destroy();
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_Game_Object(int &index, AGame_Object **object)
{
	if (index < 0 or index >= Max_Monsters_Count)
		return false;

	*object = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------