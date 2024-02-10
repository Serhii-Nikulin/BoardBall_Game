#include "Active_Brick.h"

HPEN AActive_Brick::Fading_Red_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Red_Brick_Brushes[Max_Fade_Step];
HPEN AActive_Brick::Fading_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Blue_Brick_Brushes[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type):
	Brick_Type(brick_type), Brick_Rect{}, Fade_Step(0)
{}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
	if (++Fade_Step < Max_Fade_Step)
		InvalidateRect(hwnd, &Brick_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{
	HPEN pen = 0;
	HBRUSH brush = 0;

	switch (Brick_Type)
	{
	case EBT_Red:
		pen = Fading_Red_Brick_Pens[Fade_Step];
		brush = Fading_Red_Brick_Brushes[Fade_Step];
		break;

	case EBT_Blue:
		pen = Fading_Blue_Brick_Pens[Fade_Step];
		brush = Fading_Blue_Brick_Brushes[Fade_Step];
		break;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush); 

	//Fade_Step += 1;

	Brick_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Brick_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Setup_Colors()
{
	int i;
	unsigned char r, g, b;
	int max_step = Max_Fade_Step - 1;

	for (i = 0; i < Max_Fade_Step; i++)
	{
		r = AsConfig::Red_Brick_Color.R - AsConfig::Red_Brick_Color.R * i / max_step + AsConfig::BG_Color.R * i / max_step;
		g = AsConfig::Red_Brick_Color.G - i * (AsConfig::Red_Brick_Color.G - AsConfig::BG_Color.G) / max_step;
		b = AsConfig::Red_Brick_Color.B - i * (AsConfig::Red_Brick_Color.B - AsConfig::BG_Color.B) / max_step;
																		 
		AsConfig::Create_Pen_Brush(r, g, b, Fading_Red_Brick_Pens[i], Fading_Red_Brick_Brushes[i]);							 
																		 
		r = AsConfig::Blue_Brick_Color.R - i * (AsConfig::Blue_Brick_Color.G - AsConfig::BG_Color.R) / max_step;
		g = AsConfig::Blue_Brick_Color.G - i * (AsConfig::Blue_Brick_Color.G - AsConfig::BG_Color.G) / max_step;
		b = AsConfig::Blue_Brick_Color.B - i * (AsConfig::Blue_Brick_Color.G - AsConfig::BG_Color.B) / max_step;

		AsConfig::Create_Pen_Brush(r, g, b, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
	}
}
//------------------------------------------------------------------------------------------------------------

