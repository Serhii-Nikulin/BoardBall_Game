#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
class AsConfig
{
public:
	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	static const int Global_Scale = 3;
};
//------------------------------------------------------------------------------------------------------------