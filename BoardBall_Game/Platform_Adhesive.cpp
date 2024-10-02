#include "Platform_Adhesive.h"

//AsPlatform_Adhesive
const double AsPlatform_Adhesive::Step_Adhesive_Spot_Height_Ratio = 0.04;
const double AsPlatform_Adhesive::Min_Adhesive_Spot_Height_Ratio = 0.0;
const double AsPlatform_Adhesive::Max_Adhesive_Spot_Height_Ratio = 1.0;
//------------------------------------------------------------------------------------------------------------
AsPlatform_Adhesive::AsPlatform_Adhesive(AsPlatform_State &platform_state)
	: Adhesive_Spot_Height_Ratio(0.0), Platform_State(&platform_state)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Adhesive::Act(AsBall_Set *ball_set, EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;

	switch (Platform_State->Adhesive)
	{
	case EPlatform_Transformation::Init:
		if (Adhesive_Spot_Height_Ratio < Max_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio += Step_Adhesive_Spot_Height_Ratio;
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Max_Adhesive_Spot_Height_Ratio;
			Platform_State->Adhesive = EPlatform_Transformation::Active;
		}

		return true;

	case EPlatform_Transformation::Finalize:
		if (Adhesive_Spot_Height_Ratio > Min_Adhesive_Spot_Height_Ratio)
		{
			Adhesive_Spot_Height_Ratio -= Step_Adhesive_Spot_Height_Ratio;

			while (ball_set->Release_Next_Ball() )
			{
			}
		}
		else
		{
			Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
			Platform_State->Adhesive = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
		}

		return true;
	}	

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Draw_State(HDC hdc, double x_pos)
{
	AsConfig::White_Color.Select(hdc);
	AsConfig::BG_Color.Select_Pen(hdc);

	Draw_Adhesive_Spot(hdc, x_pos, 0, 7, 4);
	Draw_Adhesive_Spot(hdc, x_pos, 5, 5, 5);
	Draw_Adhesive_Spot(hdc, x_pos, 7, 9, 5);

	AsConfig::White_Color.Select(hdc);

	Draw_Adhesive_Spot(hdc, x_pos, 0, 7, 3);
	Draw_Adhesive_Spot(hdc, x_pos, 5, 5, 4);
	Draw_Adhesive_Spot(hdc, x_pos, 7, 9, 4);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Draw_Adhesive_Spot(HDC hdc, double x_pos, int x_offset, int width, int height)
{
	RECT spot_rect{};

	int x = (int)((x_pos + 6.0) * AsConfig::D_Global_Scale);
	int y = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;

	int spot_height = int( (double)height * AsConfig::D_Global_Scale * Adhesive_Spot_Height_Ratio);

	spot_rect.left = x + x_offset * AsConfig::Global_Scale - 1;
	spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale + 1;
	spot_rect.top = y - spot_height;
	spot_rect.bottom = y + spot_height - 1;

	Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right, spot_rect.bottom, x, y, spot_rect.right, y);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Adhesive::Reset()
{
	Adhesive_Spot_Height_Ratio = Min_Adhesive_Spot_Height_Ratio;
}
//------------------------------------------------------------------------------------------------------------