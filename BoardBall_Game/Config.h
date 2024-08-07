#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//AColor
//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor();
	AColor(unsigned char r, unsigned char g, unsigned char b);
	AColor(const AColor& color, int pen_size);
	AColor(const AColor &pen_color, int pen_size, const AColor &brush_color);

	unsigned char R, G, B;

	int Get_RGB() const;
	void Select(HDC hdc) const;
	void Select_Pen(HDC hdc) const;
	HBRUSH Get_Brush() const;

private:
	HPEN Pen;
	HBRUSH Brush;
};
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush);
	static int Rand(int range);
	static void Round_Rect(HDC hdc, RECT &rect, int corner_radius = 2);
	static void Throw();

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

	static const AColor Red_Color;
	static const AColor Blue_Color;
	static const AColor BG_Color;
	static const AColor White_Color;
	static const AColor Letter_Color;
	static const AColor Red_Higlight, Blue_Highlight;
	static const AColor Teleport_Color;

	static const AColor Advert_Blue_Table_Color;
	static const AColor Advert_Red_Table_Color;

	static const int Hits_Per_Letter = 1;
	static const int Max_Falling_Letters_Count = 20;

	static const int Max_Balls_Count = 13;
};
//------------------------------------------------------------------------------------------------------------
