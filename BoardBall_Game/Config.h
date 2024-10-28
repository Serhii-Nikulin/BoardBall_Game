#pragma once
#include "Common.h"

//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush);
	
	static void Throw();

	static HWND Hwnd;
	static const int Global_Scale = 3;
	static const double D_Global_Scale;
	static const int FPS = 20;
	static int Current_Timer_Tick;
	
	static const int Border_X_Offset = 6;//Min_X
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;
	static const int Start_Ball_Position_On_Platform = (Max_X_Pos + Border_X_Offset) / 2 + 1;

	static bool Has_Floor;
	static double Moving_Step_Size;

	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	static const int Platform_Y_Pos = 185;

	static const AColor Red_Color;
	static const AColor Blue_Color;
	static const AColor BG_Color;
	static const AColor White_Color;
	static const AColor Letter_Color;
	static const AColor Laser_Color;
	static const AColor Red_Higlight, Blue_Highlight;
	static const AColor Teleport_Color;

	static const AColor Advert_Blue_Table_Color;
	static const AColor Advert_Red_Table_Color;

	static const AColor Monster_Dark_Red_Color;
	static const AColor Monster_Cornea_Color;
	static const AColor Monster_Iris_Color;
	static const AColor BG_Outline_Color;

	static const AColor Explosion_Red_Color;
	static const AColor Explosion_Blue_Color;

	static const int Hits_Per_Letter = 1;
	static const int Max_Falling_Letters_Count = 20;

	static const int Max_Balls_Count = 10;

	static const double Acceleration;
	static const double Ball_Normal_Speed;

	static const int Max_Life_Count = 12;
	static const int Initial_Life_Count = 5;

	static const int Max_Modules_Count = 10;

	static const int Platform_Normal_Width = 28;
	static const int Platform_Expanding_Inner_Width = 12;
	static const int Platform_Circle_Size = 7;
	static const int Platform_Inner_Height = 5;

	static const int Gates_Count = 8;
};
//------------------------------------------------------------------------------------------------------------
class AsTools
{
public:
	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_fade_step);
	static void Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color, int max_fade_step);

	static int Rand(int range);
	static void Rect(HDC hdc, RECT &rect, const AColor &color);
	static void Rect(HDC hdc, int x, int y, int width, int height, const AColor &color);
	static void Round_Rect(HDC hdc, RECT &rect, int corner_radius = 2);
	static void Ellipse(HDC hdc, RECT &rect, const AColor &color);
	static void Invalidate_Rect(RECT &rect);
};
//------------------------------------------------------------------------------------------------------------