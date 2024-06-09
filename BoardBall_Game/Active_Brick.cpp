#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
//AGraphics_Object
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------
//AActive_Brick
AActive_Brick::~AActive_Brick()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
	:Brick_Type(brick_type)
{
	Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width * level_x) * AsConfig::Global_Scale;
	Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * level_y) * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Red_Blue
//------------------------------------------------------------------------------------------------------------
HPEN AActive_Brick_Red_Blue::Fading_Red_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick_Red_Blue::Fading_Red_Brick_Brushes[Max_Fade_Step];
HPEN AActive_Brick_Red_Blue::Fading_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick_Red_Blue::Fading_Blue_Brick_Brushes[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y):
	AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
	if (! (brick_type == EBT_Blue or brick_type == EBT_Red))
		throw 13;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Act()
{
	if (++Fade_Step < Max_Fade_Step)
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
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

	default:
		throw 13;
	}

	if (pen and brush)
	{
		if (IntersectRect(&intersection_rect, &paint_area, &Brick_Rect))
		{
			SelectObject(hdc, pen);
			SelectObject(hdc, brush); 

			RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Red_Blue::Is_Finished()
{
	if (Fade_Step == Max_Fade_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Setup_Colors()
{
	int i;

	for (i = 0; i < Max_Fade_Step; i++)
	{
		Get_Fading_Color(AsConfig::Red_Brick_Color, i, AActive_Brick_Red_Blue::Fading_Red_Brick_Pens[i], AActive_Brick_Red_Blue::Fading_Red_Brick_Brushes[i]);
		Get_Fading_Color(AsConfig::Blue_Brick_Color, i, AActive_Brick_Red_Blue::Fading_Blue_Brick_Pens[i], AActive_Brick_Red_Blue::Fading_Blue_Brick_Brushes[i]);
	}
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - color * step / (Max_Fade_Step - 1) + bg_color * step / (Max_Fade_Step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush)
{
	unsigned char r, g, b;
	r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);
	AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
}
// //------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(EBrick_Type brick_type, int level_x, int level_y):
	AActive_Brick(brick_type, level_x, level_y), Unbreakable_Animation_Step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT &paint_rect)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
	if (Unbreakable_Animation_Step < Max_Unbreakable_Animation_Step)
	{
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
		Unbreakable_Animation_Step += 1;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
