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
AColor AActive_Brick_Red_Blue::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y) :
	AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
	if (!(brick_type == EBT_Blue or brick_type == EBT_Red))
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
	AColor* color = 0;

	switch (Brick_Type)
	{
	case EBT_Red:
		color = &Fading_Red_Brick_Colors[Fade_Step];
		break;

	case EBT_Blue:
		color = &Fading_Blue_Brick_Colors[Fade_Step];
		break;

	default:
		AsConfig::Throw();
	}

	//if (IntersectRect(&intersection_rect, &paint_area, &Brick_Rect))
	{
		if (color != 0)
			color->Select(hdc);

		AsConfig::Round_Rect(hdc, Brick_Rect);
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
		Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
		Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{
	const AColor *color = 0;

	switch (brick_type)
	{
	case EBT_None:
		color = &AsConfig::BG_Color;
		break;

	case EBT_Blue:
		color = &AsConfig::Blue_Color;
		break;

	case EBT_Red:
		color = &AsConfig::Red_Color;
		break;

	case EBT_Unbreakable:
		color = &AsConfig::White_Color;
		break;

	default:
		AsConfig::Throw();
	}

	if (color != 0)
		color->Select(hdc);

	AsConfig::Round_Rect(hdc, brick_rect);
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - color * step / (Max_Fade_Step - 1) + bg_color * step / (Max_Fade_Step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

	result_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
AColor AActive_Brick_Unbreakable::Red_Higlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
AColor AActive_Brick_Unbreakable::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
	DeleteObject(Region);
}
// //------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(EBrick_Type brick_type, int level_x, int level_y) :
	AActive_Brick(brick_type, level_x, level_y), Animation_Step(0)
{
	Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_rect)
{
	const int scale = AsConfig::Global_Scale;
	int offset;

	Draw_In_Level(hdc, Brick_Rect);

	AsConfig::Red_Color.Select(hdc);

	SelectClipRgn(hdc, Region);

	Red_Higlight.Select_Pen(hdc);

	offset = (2 * Animation_Step - AsConfig::Brick_Width) * scale;

	MoveToEx(hdc, Brick_Rect.left + 2 * scale + offset, Brick_Rect.top + 8 * scale, 0);
	LineTo(hdc, Brick_Rect.left + 11 * scale + offset, Brick_Rect.top - 1 * scale);

	Blue_Highlight.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left + 0 * scale + offset, Brick_Rect.top + 8 * scale, 0);
	LineTo(hdc, Brick_Rect.left + 10 * scale + offset, Brick_Rect.top - 2 * scale);

	SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
	if (Animation_Step < Max_Animation_Step)
	{
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
		Animation_Step += 1;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
	if (Animation_Step >= Max_Animation_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT &brick_rect)
{
	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Multihit
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{

}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::AActive_Brick_Multihit(EBrick_Type brick_type, int level_x, int level_y)
	:AActive_Brick(brick_type, level_x, level_y)
{

}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw(HDC hdc, RECT &paint_rect)
{
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Multihit::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{
	const int scale = AsConfig::Global_Scale;
	

	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	AsConfig::Red_Color.Select(hdc);
	Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

	switch (brick_type)
	{
	case EBT_Multihit_1:
		Draw_Stage(hdc, brick_rect, 2, 10);
		break;
		
	case EBT_Multihit_2:
		Draw_Stage(hdc, brick_rect, 2, 4);
		Draw_Stage(hdc, brick_rect, 8, 4);
		break;

	case EBT_Multihit_3:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 6, 2);
		Draw_Stage(hdc, brick_rect, 10, 2);
		break;

	case EBT_Multihit_4:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 5, 2);
		Draw_Stage(hdc, brick_rect, 8, 2);
		Draw_Stage(hdc, brick_rect, 11, 2);
		break;

	default:
		AsConfig::Throw();
	}

}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT &brick_rect, int x, int width)
{
	RECT stage_rect;
	const int scale = AsConfig::Global_Scale;

	stage_rect.left = brick_rect.left + x * scale;
	stage_rect.top = brick_rect.top + 2 * scale;
	stage_rect.right = stage_rect.left + width * scale - 1;
	stage_rect.bottom = stage_rect.top + 3 * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale, stage_rect.bottom + scale);

	AsConfig::Blue_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right, stage_rect.bottom);
}
//------------------------------------------------------------------------------------------------------------

