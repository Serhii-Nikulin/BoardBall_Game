#include "Border.h"
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
{
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 3 + i * 4, 0, true);//top horizontal part

	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 2, 1 + i * 4, false);//left part

	for (i = 0; i < 50; ++i)
		Draw_Element(hdc, 201, 1 + i * 4, false);//right part
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

	if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
	{
		if (AsConfig::Has_Floor)
		{
			got_hit = true;
			ball->Reflect(true);//from low horizontal
		}
		else
		{
			if (next_y_pos - ball->Radius * 4 > AsConfig::Max_Y_Pos)
				ball->Set_State(EBS_Lost);
		}
	}

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{
	AsConfig::White_Color.Select(hdc);

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
