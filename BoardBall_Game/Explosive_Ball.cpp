#include "Explosive_Ball.h"

//AExplosive_Ball
//------------------------------------------------------------------------------------------------------------
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