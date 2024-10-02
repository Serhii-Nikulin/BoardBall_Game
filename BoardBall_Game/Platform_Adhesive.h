#pragma once
#include "Platform_State.h"
#include "Ball_Set.h"

//------------------------------------------------------------------------------------------------------------
class AsPlatform_Adhesive
{
public:
	AsPlatform_Adhesive(AsPlatform_State &platform_state);
	bool Act(AsBall_Set *ball_set, EPlatform_State &next_state);

	void Draw_State(HDC hdc, double x_pos);
	void Draw_Adhesive_Spot(HDC hdc, double x_pos, int x_offset, int width, int heigth);
	void Reset();

private:
	double Adhesive_Spot_Height_Ratio;

	static const double Step_Adhesive_Spot_Height_Ratio;
	static const double Min_Adhesive_Spot_Height_Ratio;
	static const double Max_Adhesive_Spot_Height_Ratio;

	AsPlatform_State *Platform_State;
};
//------------------------------------------------------------------------------------------------------------