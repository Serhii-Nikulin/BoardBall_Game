#pragma once
#include "Config.h"

enum EBrick_Type {
	EBT_None, EBT_Red, EBT_Blue
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick
{
public:
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);
	void Act();
	void Draw(HDC hdc);
	static void Setup_Colors();
	RECT Brick_Rect;

private:
	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush);

	EBrick_Type Brick_Type;
	int Fade_Step;

	static const int Max_Fade_Step = 80;
	static HPEN Fading_Red_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Red_Brick_Brushes[Max_Fade_Step];
	static HPEN Fading_Blue_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Max_Fade_Step];
};
//------------------------------------------------------------------------------------------------------------
