#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	static const int Global_Scale = 3;

	static const int Border_X_Offset = 6;//Min_X
	static const int Border_Y_Offset = 4;//Min_Y
	static const int Max_X_Pos = 200;
	static const int Max_Y_Pos = 199;

	static const int Y_Pos = 185;
};
//------------------------------------------------------------------------------------------------------------
