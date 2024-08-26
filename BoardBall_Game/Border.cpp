#include "Border.h"
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
{
	Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.top = (AsConfig::Max_Y_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.right = AsConfig::Max_X_Pos * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
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

	if (next_y_pos - 2 * ball->Radius > AsConfig::Max_Y_Pos)
	{
		if (next_y_pos - ball->Radius * 2 > AsConfig::Max_Y_Pos)
			ball->Set_State(EBS_Lost);
	}

 	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	InvalidateRect(AsConfig::Hwnd, &Floor_Rect, FALSE);
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