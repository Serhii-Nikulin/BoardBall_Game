#pragma once
#include "Border.h"
#include "Ball.h"
#include "Platform.h"

enum EKey_Type {
	EKT_Left, EKT_Right, EKT_Space
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	static void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush);
	HWND Hwnd;

private:
	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------


