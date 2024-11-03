#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class EExplosive_Ball_State: unsigned char
{
	Idle,
	Charging,
	Expanding,
	Fading
};
//------------------------------------------------------------------------------------------------------------
class AExplosive_Ball: public AGraphics_Object
{
public:
	AExplosive_Ball();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Explode(int x_pos, int y_pos, double max_size, bool is_red, int time_offset, int count_count);
	static void Setup_Colors();

private:
	void Update_Exploding_Ball_Rect();

	int X_Pos, Y_Pos;
	double Size, Max_Size;
	int Step_Count;
	int Time_Offset;
	double Size_Step;
	int Start_Fading_Tick;
	int Start_Expanding_Tick;
	bool Is_Red;
	static const int Fading_Timeout = AsConfig::FPS;
	static const int Max_Fade_Step = AsConfig::FPS;
	static AColor Fading_Red_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Colors[Max_Fade_Step];

	EExplosive_Ball_State Explosive_Ball_State;
	RECT Explosive_Ball_Rect;
};
//------------------------------------------------------------------------------------------------------------