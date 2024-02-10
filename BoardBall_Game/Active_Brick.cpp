#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick():
	Brick_Rect{}, Fade_Step(0)
{}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
	if (Fade_Step >= Max_Fade_Step)
		return;

	Fade_Step += 1;
	InvalidateRect(hwnd, &Brick_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{
	HPEN pen;
	HBRUSH brush;

	unsigned char r = AsConfig::Blue_Brick_Color.R - AsConfig::Blue_Brick_Color.R * Fade_Step / Max_Fade_Step;
	unsigned char g = AsConfig::Blue_Brick_Color.G - AsConfig::Blue_Brick_Color.G * Fade_Step / Max_Fade_Step;
	unsigned char b = AsConfig::Blue_Brick_Color.B - AsConfig::Blue_Brick_Color.B * Fade_Step / Max_Fade_Step;;

	AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush); 

	Fade_Step += 1;

	Brick_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Brick_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
