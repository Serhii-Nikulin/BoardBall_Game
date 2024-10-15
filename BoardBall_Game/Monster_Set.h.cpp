#include "Monster_Set.h"

//AMonster
//------------------------------------------------------------------------------------------------------------
const double AMonster::Blink_Timeouts[AMonster::Blink_Stages_Count] = { 0.5, 0.5, 1.0, 0.5, 
	0.5, 0.5, 1.2};
const EEye_State AMonster::Blink_States[AMonster::Blink_Stages_Count] = {EEye_State::Closed, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing, EEye_State::Opening, EEye_State::Staring, EEye_State::Closing};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
	: Is_Active(false), Monster_Rect{}, X_Pos(0), Y_Pos(0), Cornea_Height(0), Eye_State(EEye_State::Closed), Blink_Ticks{}, Start_Blink_Timeout(0), Total_Animation_Timeout(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Shift_Per_Step(double max_speed)
{
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	int i;
	int current_tick_offset, prev_tick;
	double ratio;

	if (! Is_Active)
		return;

	current_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blink_Timeout) % Total_Animation_Timeout;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		if (current_tick_offset < Blink_Ticks[i])
		{
			Eye_State = Blink_States[i];
			break;
		}	
	}

	if (i == 0)
		prev_tick = 0;
	else
		prev_tick = Blink_Ticks[i - 1];

	ratio = ((double)current_tick_offset - prev_tick) / (Blink_Ticks[i] - prev_tick);

	switch (Eye_State)
	{
	case EEye_State::Closed:
		Cornea_Height = 0.0;
		break;

	case EEye_State::Opening:
		Cornea_Height = ratio * Max_Cornea_Height;
		break;

	case EEye_State::Staring:
		Cornea_Height = Max_Cornea_Height;
		break;

	case EEye_State::Closing:
		Cornea_Height = (1.0 - ratio) * Max_Cornea_Height;
		break;

	default:
		AsConfig::Throw();
	}

	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;
	HRGN region;

	RECT rect, cornea_rect;
	const int scale = AsConfig::Global_Scale;
	const int half_scale = scale / 2;

	if (! Is_Active)
		return;

	if (! IntersectRect(&intersection_rect, &Monster_Rect, &paint_area) )
		return;

	rect = Monster_Rect;
	rect.right += 1;
	rect.bottom += 1;

	region = CreateEllipticRgnIndirect(&rect);

	SelectClipRgn(hdc, region);
	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Monster_Dark_Red_Color);

	rect = Monster_Rect;

	rect.left -= 2 * scale;
	rect.right -= 2 * scale;
	rect.top -= 3 * scale;
	rect.bottom -= 3 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);
	
	if (Eye_State == EEye_State::Closed)
		return;

	//Draw cornea
	cornea_rect = Monster_Rect;
	cornea_rect.left += 1 * scale + half_scale;
	cornea_rect.right -= 1 * scale + half_scale;
	cornea_rect.top += int( (Height - Cornea_Height) / 2.0 * AsConfig::D_Global_Scale);
	cornea_rect.bottom -= int( (Height - Cornea_Height) / 2.0 * AsConfig::D_Global_Scale);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	region = CreateEllipticRgnIndirect(&cornea_rect);
	SelectClipRgn(hdc, region);

	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Monster_Cornea_Color);

	//Draw iris
	rect = Monster_Rect;
	rect.left += 4 * scale + half_scale;
	rect.right -= 4 * scale + half_scale;
	rect.top += 5 * scale;
	rect.bottom -= 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Monster_Iris_Color);

	//Draw pupil
	rect = Monster_Rect;
	rect.left += 7 * scale;
	rect.right = rect.left + 2 * scale;
	rect.top += 6 * scale + half_scale;
	rect.bottom = rect.top + 3 * scale + half_scale;

	AsTools::Ellipse(hdc, rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	AsConfig::BG_Outline_Color.Select(hdc);
	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right - 1, cornea_rect.bottom - 1, 0, 0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Activate(int x_pos, int y_pos)
{
	int i;
	const int scale = AsConfig::Global_Scale;
	double current_timeout = 0.0;

	if (Is_Active)
		return;

	Is_Active = true;
	X_Pos = x_pos + 10;
	Y_Pos = y_pos;

	Monster_Rect.left = X_Pos * scale;
	Monster_Rect.top = Y_Pos * scale;
	Monster_Rect.right = Monster_Rect.left + Width * scale;
	Monster_Rect.bottom = Monster_Rect.top + Height * scale;

	Start_Blink_Timeout = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		current_timeout += Blink_Timeouts[i];
		Blink_Ticks[i] = current_timeout * AsConfig::FPS;
	}

	Total_Animation_Timeout = Blink_Ticks[i - 1];
}
//------------------------------------------------------------------------------------------------------------




//AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
: Border(0), Monsters{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder *border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_From_Gate(int gate_index)
{
	int i;
	int gate_x_pos, gate_y_pos;

	AMonster *monster = 0;

	if (gate_index < 0 or gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (! Monsters[i].Is_Active)
		{
			monster = &Monsters[i];
			break;
		}
	}

	if (monster == 0)
		return;

	Border->Get_Gate_Pos(gate_index, gate_x_pos, gate_y_pos);
	monster->Activate(gate_x_pos, gate_y_pos);
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_Game_Object(int &index, AGame_Object **object)
{
	if (index < 0 or index >= Max_Monsters_Count)
		return false;

	*object = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------