#pragma once
#include "Platform_State.h"

//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State() :
	Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing), Meltdown(EPlatform_Substate_Meltdown::Unknown), Rolling(EPlatform_Substate_Rolling::Unknown), Adhesive(EPlatform_Transformation::Unknown), Expanding(EPlatform_Transformation::Unknown), Laser(EPlatform_Transformation::Unknown), Moving_State(EPlatform_Moving_State::Stop)
{
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::operator EPlatform_State() const
{
	return Current_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::operator = (const EPlatform_State &new_platform_state)
{
	Current_State = new_platform_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
	if (next_state == Current_State)
		return;

	switch (Current_State)
	{
	case EPlatform_State::Regular:
		AsConfig::Throw();

	case EPlatform_State::Meltdown:
		break;

	case EPlatform_State::Rolling:
		AsConfig::Throw();

	case EPlatform_State::Adhesive:
		Adhesive = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Expanding:
		Expanding = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Laser:
		Laser = EPlatform_Transformation::Finalize;
		break;

	default:
		AsConfig::Throw();
	}

	Next_State = next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Get_Next_State() const
{
	return Next_State;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_State next_state = Get_Next_State();
	Current_State = EPlatform_State::Regular;

	if (next_state == EPlatform_State::Unknown)
		Regular = new_regular_state;

	return next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_Transformation *transformation_state = 0;
	EPlatform_State next_state;

	if (Current_State == EPlatform_State::Regular and Regular == new_regular_state)
		return EPlatform_State::Unknown;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		switch (Current_State)
		{
		case EPlatform_State::Adhesive:
			transformation_state = &Adhesive;
			break;

		case EPlatform_State::Expanding:
			transformation_state = &Expanding;
			break;

		case EPlatform_State::Laser:
			transformation_state = &Laser;
			break;
		}

		if (transformation_state != 0)
		{
			if (*transformation_state == EPlatform_Transformation::Unknown)
			{
				next_state = Set_Next_Or_Regular_State(new_regular_state);

				return next_state;
			}
			else 
				if(*transformation_state == EPlatform_Transformation::Active or *transformation_state == EPlatform_Transformation::Init)
					*transformation_state =  EPlatform_Transformation::Finalize;

			return EPlatform_State::Unknown;
		}
	}

	Current_State = EPlatform_State::Regular;
	Regular = new_regular_state;

	return EPlatform_State::Unknown;
}
//------------------------------------------------------------------------------------------------------------