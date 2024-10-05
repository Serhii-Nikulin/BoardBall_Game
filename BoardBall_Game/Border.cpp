#include "Border.h"
#include "Config.h"


//AGate
//------------------------------------------------------------------------------------------------------------
AGate::AGate(int x_pos, int y_pos)
: X_Pos(x_pos), Y_Pos(y_pos)
{
	int scale = AsConfig::Global_Scale;

	Rect.left = X_Pos * scale;
	Rect.top = Y_Pos * scale;
	Rect.right = Rect.left + Width * scale;
	Rect.bottom = Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Act()
{
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;
	if (! IntersectRect(&intersection_rect, &paint_area, &Rect) )
		return;

	Clear_Prev_Animation(hdc, paint_area);
	Draw_Cup(hdc, true);
	Draw_Cup(hdc, false);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Cup(HDC hdc, bool is_top)
{
	const int scale = AsConfig::Global_Scale;
	double half_scale = scale / 2.0;
	HRGN region;
	RECT rect, region_rect;
	XFORM xform, prev_xform;

	xform.eM11 = (FLOAT)1.0; xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0; 
	xform.eDx = X_Pos * scale;

	if(is_top)
	{
		xform.eM22 = (FLOAT)1.0;
		xform.eDy = Y_Pos * scale;
	}
	else
	{
		xform.eM22 = (FLOAT)-1.0;
		xform.eDy = (Y_Pos + Height) * scale - 1;
	}

	GetWorldTransform(hdc, &prev_xform);
	SetWorldTransform(hdc, &xform);
	
	AsConfig::Rect(hdc, 2, 0, 2, 1, AsConfig::Blue_Color);//holder

	region_rect.left = X_Pos * scale;
	region_rect.right = region_rect.left + 6 * scale;

	if (is_top)
	{
		region_rect.top = (Y_Pos + 1) * scale;
		region_rect.bottom = region_rect.top + 4 * scale;
	}
	else
	{
		region_rect.top = (Y_Pos + Height - 1) * scale;
		region_rect.bottom = region_rect.top - 4 * scale;
	}

	region = CreateRectRgnIndirect(&region_rect);
	SelectClipRgn(hdc, region);

	rect.left = 0 * scale;
	rect.top = (0 + 1) * scale;
	rect.right = rect.left + 6 * scale;
	rect.bottom = rect.top + 6 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	region_rect.right = region_rect.left + 3 * scale;

	region = CreateRectRgnIndirect(&region_rect);
	SelectClipRgn(hdc, region);

	rect.left = 0 * scale + half_scale;
	rect.top = 1 * scale + half_scale;
	rect.right = rect.left + 5 * scale;
	rect.bottom = rect.top + 6 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsConfig::Letter_Color.Select_Pen(hdc);
	AsConfig::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	AsConfig::Rect(hdc, 4, 3, 1, 1, AsConfig::BG_Color);//bg_perforation
	
	int i;

	int x = 0;
	int y = 4;

	for (i = 0; i < 3; i++)
	{
		Draw_Edge(hdc, x, y, true);
		Draw_Edge(hdc, x, y, false);

		y += 2;
	}

	SetWorldTransform(hdc, &prev_xform);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Edge(HDC hdc, int x, int y, bool is_longer)
{
	const int scale = AsConfig::Global_Scale;

	if (is_longer)
	{
		AsConfig::Rect(hdc, x, y, 4, 1, AsConfig::White_Color);
		AsConfig::Rect(hdc, x + 4, y, 2, 1, AsConfig::Blue_Color);
	}
	else
	{
		AsConfig::Rect(hdc, x + 1, y + 1, 2, 1, AsConfig::Blue_Color);
		AsConfig::Rect(hdc, x + 4, y + 1, 1, 1, AsConfig::Blue_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	AsConfig::BG_Color;
	AsConfig::Rect(hdc, Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------




//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::~AsBorder()
{
	int i;
	
	for (i = 0; i < AsConfig::Gates_Counter; i++)
	{
		delete Gates[i];
		Gates[i] = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
	:Gates{}
{
	Gates[0] = new AGate(1, 29);
	Gates[1] = new AGate(AsConfig::Max_X_Pos, 29);
	Gates[2] = new AGate(1, 77);
	Gates[3] = new AGate(AsConfig::Max_X_Pos, 77);
	Gates[4] = new AGate(1, 129);
	Gates[5] = new AGate(AsConfig::Max_X_Pos, 129);
	Gates[6] = new AGate(1, 178);
	Gates[7] = new AGate(AsConfig::Max_X_Pos, 178);

	Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.top = (AsConfig::Max_Y_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.right = AsConfig::Max_X_Pos * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	bool got_hit = false;

	if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);//from left vertical
	}

	if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect(true);//from top horizontal
	}

	if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
	{
		got_hit = true;
		ball->Reflect(false);//from right vertical
	}

	if (AsConfig::Has_Floor)
		if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos - 1)
		{
			got_hit = true;
			ball->Reflect(true);//from low horizontal
		}

	if (next_y_pos - ball->Radius * 4 > AsConfig::Max_Y_Pos)
		ball->Set_State(EBS_Lost);

 	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{//code stub
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 3 + i * 4, 0, true, paint_area);//top horizontal part

	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 2, 1 + i * 4, false, paint_area);//left part

	
	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 201, 1 + i * 4, false, paint_area);//right part
	

	if (AsConfig::Has_Floor)
		Draw_Floor(hdc, paint_area);

	for (i = 0; i < AsConfig::Gates_Counter; i++)
		Gates[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (AsConfig::Has_Floor)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	AsConfig::BG_Color.Select(hdc);

	AsConfig::Round_Rect(hdc, Floor_Rect, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsConfig::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area)
{
	RECT intersection_rect, rect;
	AsConfig::White_Color.Select(hdc);

	rect.left = x * AsConfig::Global_Scale;
	rect.top = y * AsConfig::Global_Scale;
	rect.right = (x + 4) * AsConfig::Global_Scale;
	rect.bottom = (y + 4) * AsConfig::Global_Scale;

	if (! IntersectRect(&intersection_rect, &rect, &paint_area) )
		return;

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	AsConfig::Blue_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1 + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1 + 3) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	AsConfig::BG_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area)
{
	int i;
	int x, y;
	int storkes;
	int stroke_length, gape_length, full_length;
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	stroke_length= 6 * AsConfig::Global_Scale;
	gape_length = 2 * AsConfig::Global_Scale;
	full_length = gape_length + stroke_length;
	storkes = (Floor_Rect.right - Floor_Rect.left) / full_length;
	x = Floor_Rect.left + 1;
	y = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;
	AsConfig::Letter_Color.Select(hdc);

	for (i = 0; i < storkes; i++)
	{
		MoveToEx(hdc, x, y, FALSE);
		LineTo(hdc, x + stroke_length, y);
		x += full_length;
	}
}
//------------------------------------------------------------------------------------------------------------