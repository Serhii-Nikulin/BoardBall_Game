#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//AColor
//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor(unsigned char r, unsigned char g, unsigned char b);
	const unsigned char R, G, B;

	int Get_RGB() const;
};
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush);
	static void Setup_Colors();
	static int Rand(int range);

	static HWND Hwnd;
	static const int Global_Scale = 3;
	static const int FPS = 20;
	static int Current_Timer_Tick;

	static const int Border_X_Offset = 6;//Min_X
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;

	static bool Has_Floor;
	static double Moving_Step_Size;

	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

	static const int Platform_Y_Pos = 185;

	static const AColor Red_Brick_Color;
	static const AColor Blue_Brick_Color;
	static const AColor BG_Color;

	static const int Hits_Per_Letter = 1;
	static const int Max_Falling_Letters_Count = 10;

	static HPEN BG_Pen;
	static HBRUSH BG_Brush;

	static HPEN Letter_Pen;
	static HPEN Brick_Red_Pen, Brick_Blue_Pen;
	static HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
};
//------------------------------------------------------------------------------------------------------------
