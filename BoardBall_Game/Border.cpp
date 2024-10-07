#include "Border.h"
#include "Config.h"

//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::~AsBorder()
{
	int i;
	
	for (i = 0; i < AsConfig::Gates_Count; i++)
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
		ball->Set_State(EBall_State::Lost);

 	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Act();
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

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	int i;
	RECT intersection_rect;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		Gates[i]->Clear_Prev_Animation(hdc, paint_area);

	if (AsConfig::Has_Floor)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	AsConfig::BG_Color.Select(hdc);

	AsTools::Round_Rect(hdc, Floor_Rect, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsTools::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Open_Gate(int gate_index, bool short_open)
{
	if ( (gate_index != AsConfig::Gates_Count - 1) and short_open)
		AsConfig::Throw();

	if (gate_index >= 0 and gate_index < AsConfig::Gates_Count)
		Gates[gate_index]->Open_Gate(short_open);
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Opened(int gate_index)
{
	if (gate_index >= 0 and gate_index < AsConfig::Gates_Count)
		return Gates[gate_index]->Is_Opened();
	else
	{
		AsConfig::Throw();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area)
{
	int i;
	RECT intersection_rect{}, rect{};
	int gate_top_y, gate_low_y;

	rect.left = x * AsConfig::Global_Scale;
	rect.top = y * AsConfig::Global_Scale;
	rect.right = (x + 4) * AsConfig::Global_Scale;
	rect.bottom = (y + 4) * AsConfig::Global_Scale;

	for (i = 0; i < AsConfig::Gates_Count; i++)
	{
		Gates[i]->Get_Y_Limits(gate_top_y, gate_low_y);

		if (rect.top >= gate_top_y and rect.bottom <= gate_low_y)
			return;
	}

	if (! IntersectRect(&intersection_rect, &rect, &paint_area) )
		return;

	if (top_border)
		AsTools::Rect(hdc, x, y, 4, 1, AsConfig::White_Color);
	else
		AsTools::Rect(hdc, x, y, 1, 4, AsConfig::White_Color);

	if (top_border)
		AsTools::Rect(hdc, x, y + 1, 4, 3, AsConfig::Blue_Color);
	else
		AsTools::Rect(hdc, x + 1, y, 3, 4, AsConfig::Blue_Color);

	if (top_border)
		AsTools::Rect(hdc, x + 2, y + 2, 1, 1, AsConfig::BG_Color);
	else
		AsTools::Rect(hdc, x + 2, y + 1, 1, 1, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area) const
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
