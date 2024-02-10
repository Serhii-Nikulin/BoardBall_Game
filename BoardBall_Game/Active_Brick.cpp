#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick():
	Brick_Rect{}, Brick_Pen{}, Brick_Brush{}, Fade_Step(0)
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
	AsConfig::Create_Pen_Brush(63 - 63 * Fade_Step / Max_Fade_Step, 72 - 72 * Fade_Step / Max_Fade_Step, 204 - 204 * Fade_Step / Max_Fade_Step, Brick_Pen, Brick_Brush);
	SelectObject(hdc, Brick_Pen);
	SelectObject(hdc, Brick_Brush); 
	Fade_Step += 1;

	Brick_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Brick_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
