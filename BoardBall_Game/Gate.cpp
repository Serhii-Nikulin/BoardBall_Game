#include "Gate.h"

//AGate
//------------------------------------------------------------------------------------------------------------
const double AGate::Max_Gap_Short_Height = 9.0;
const double AGate::Gap_Height_Short_Step = Max_Gap_Short_Height / (double)AsConfig::FPS;

const double AGate::Max_Gap_Long_Height = 18.0;
const double AGate::Gap_Height_Long_Step = Max_Gap_Long_Height / (3.0 * (double)AsConfig::FPS);
//------------------------------------------------------------------------------------------------------------
AGate::AGate(int x_pos, int y_pos)
	: X_Pos(x_pos), Y_Pos(y_pos), Origin_Y_Pos(y_pos), Gate_State(EGate_State::Closed), Gate_Transformation(EGate_Transformation::Unknown), Gap_Height(0.0), Gate_Close_Tick(0)
{
	int scale = AsConfig::Global_Scale;

	Rect.left = X_Pos * scale;
	Rect.top = int(Y_Pos * AsConfig::D_Global_Scale);
	Rect.right = Rect.left + Width * scale;
	Rect.bottom = Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Act()
{
	bool correct_pos = false;

	switch(Gate_State)
	{
	case EGate_State::Closed:
		break;

	case EGate_State::Short_Open:
		if (Act_For_Open(true, correct_pos) )
			Redraw_Gate();
		break;

	case EGate_State::Long_Open:
		if (Act_For_Open(false, correct_pos) )
		{
			if (correct_pos)
			{
				Y_Pos = Origin_Y_Pos - Gap_Height / 2.0;
				Rect.top = int(Y_Pos * AsConfig::D_Global_Scale);
				Rect.bottom = int( (Y_Pos + Height + Gap_Height) * AsConfig::D_Global_Scale);
			}
			Redraw_Gate();
		}
		break;

	default:
		AsConfig::Throw();
	}
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

	if (Gate_State == EGate_State::Long_Open and (Gate_Transformation == EGate_Transformation::Init or Gate_Transformation == EGate_Transformation::Finalize) )
		Draw_Blazing(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Cup(HDC hdc, bool is_top)
{
	const double d_scale = AsConfig::D_Global_Scale;
	const int scale = AsConfig::Global_Scale;
	double half_scale = scale / 2.0;
	HRGN region;
	RECT rect{}, region_rect{};
	XFORM xform{}, prev_xform{};

	double reverse_height = int( (Gap_Height + Height) * d_scale - 1.0);

	xform.eM11 = (FLOAT)1.0; xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0; 
	xform.eDx = (FLOAT)(X_Pos * scale);

	if (is_top)
	{
		xform.eM22 = (FLOAT)1.0;
		xform.eDy = (FLOAT)(Y_Pos * scale);
	}
	else
	{
		xform.eM22 = (FLOAT)-1.0;

		if (Gate_State == EGate_State::Long_Open)
			xform.eDy = FLOAT(Y_Pos * d_scale + reverse_height);
		else
			xform.eDy = FLOAT( (Y_Pos + Height) * scale - 1.0);
	}

	GetWorldTransform(hdc, &prev_xform);
	SetWorldTransform(hdc, &xform);

	AsTools::Rect(hdc, 2, 0, 2, 1, AsConfig::Blue_Color);//holder

	region_rect.left = X_Pos * scale;
	region_rect.right = region_rect.left + 6 * scale;

	if (is_top)
	{
		region_rect.top = int( (Y_Pos + 1.0) * d_scale);
		region_rect.bottom = region_rect.top + 4 * scale;
	}
	else
	{
		if (Gate_State == EGate_State::Long_Open)
			region_rect.top = int(Y_Pos * d_scale + reverse_height);

		else
			region_rect.top = int( (Y_Pos + Height - 1.0) * d_scale);

		region_rect.bottom = region_rect.top - 4 * scale;
	}

	region = CreateRectRgnIndirect(&region_rect);
	SelectClipRgn(hdc, region);

	rect.left = 0 * scale;
	rect.top = (0 + 1) * scale;
	rect.right = rect.left + 6 * scale;
	rect.bottom = rect.top + 6 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	region_rect.right = region_rect.left + 3 * scale;

	region = CreateRectRgnIndirect(&region_rect);
	SelectClipRgn(hdc, region);

	rect.left = 0 * scale + (LONG)half_scale;
	rect.top = 1 * scale + (LONG)half_scale;
	rect.right = rect.left + 5 * scale;
	rect.bottom = rect.top + 6 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsConfig::Letter_Color.Select_Pen(hdc);
	AsTools::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	AsTools::Rect(hdc, 4, 3, 1, 1, AsConfig::BG_Color);//bg_perforation

	Draw_Edges(hdc);

	SetWorldTransform(hdc, &prev_xform);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Edges(HDC hdc)
{
	int i;
	bool is_longer_edge;
	int x, y;

	if (Gate_State == EGate_State::Short_Open)
		Draw_Short_Open_Edges(hdc);
	else if (Gate_State == EGate_State::Long_Open)
		Draw_Long_Open_Edges(hdc);
	else
	{
		x = 0;
		y = 4;
		is_longer_edge = true;

		for (i = 0; i < AsConfig::Gates_Count; i++)
		{
			Draw_One_Edge(hdc, x, y, is_longer_edge);
			is_longer_edge = !is_longer_edge;
			y += 1;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Short_Open_Edges(HDC hdc)
{
	int i;
	int x = 0;
	int y = 4;
	int count;
	double ratio;
	bool is_longer_edge;

	ratio = 1.0 - Gap_Height / Max_Gap_Short_Height + 1.0 / Edges_Count_Per_Cup;
	count = int(Edges_Count_Per_Cup * ratio);
	is_longer_edge = true;

	for (i = 0; i < count; i++)
	{
		Draw_One_Edge(hdc, x, y, is_longer_edge);
		is_longer_edge = !is_longer_edge;
		y += 1;
	}
}
//-----------------------------------------------------------------------------------------------------------
void AGate::Draw_Long_Open_Edges(HDC hdc)
{
	int i;
	int x = 0;
	int y = 4;
	double ratio;
	bool is_longer_edge;

	ratio = Gap_Height / Max_Gap_Long_Height;

	if (ratio < 0.3)
	{
		is_longer_edge = true;
		for (i = 0; i < 4; i++)
		{
			Draw_One_Edge(hdc, x, y++, is_longer_edge);
			is_longer_edge = !is_longer_edge;
		}

		if (ratio > 0.1)
			Draw_Red_Edge(hdc, x, y++, false, false);
	}
	else if (ratio < 0.5)
	{
		Draw_One_Edge(hdc, x, y++, false);
		Draw_One_Edge(hdc, x, y++, true);
		Draw_One_Edge(hdc, x, y++, true);

		Draw_Red_Edge(hdc, x, y++, true, true);
		Draw_Red_Edge(hdc, x, y++, false, false);
	}
	else
	{
		for (i = 0; i < 3; i++)
			Draw_One_Edge(hdc, x, y++, true);

		Draw_Red_Edge(hdc, x, y++, true, false);
		Draw_Red_Edge(hdc, x, y++, true, true);
		Draw_Red_Edge(hdc, x, y++, false, false);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_One_Edge(HDC hdc, int x, int y, bool is_longer_edge)
{
	const int scale = AsConfig::Global_Scale;

	if (is_longer_edge)
	{
		AsTools::Rect(hdc, x, y, 4, 1, AsConfig::White_Color);
		AsTools::Rect(hdc, x + 4, y, 2, 1, AsConfig::Blue_Color);
	}
	else
	{
		AsTools::Rect(hdc, x + 1, y, 2, 1, AsConfig::Blue_Color);
		AsTools::Rect(hdc, x + 4, y, 1, 1, AsConfig::Blue_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Red_Edge(HDC hdc, int x, int y, bool is_longer_edge, bool has_highlight)
{
	if (is_longer_edge)
	{
		AsTools::Rect(hdc, x, y, 6, 1, AsConfig::Red_Color);

		if (has_highlight)
			AsTools::Rect(hdc, x + 1, y, 1, 1, AsConfig::White_Color);
	}
	else
		AsTools::Rect(hdc, x + 1, y, 4, 1, AsConfig::Red_Color);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Blazing(HDC hdc)
{
	int i;
	double ratio;
	int dots_count = 5;
	int x, y;
	int y_offset;

	ratio = Gap_Height / Max_Gap_Long_Height;

	if (ratio < 0.2 or ratio > 0.9)
		return;

	y_offset = (int)(Origin_Y_Pos + Height / 2.0 - Gap_Height / 2.0) + 1;

	for (i = 0; i < dots_count; i++)
	{
		x = 1 + AsTools::Rand(4);
		y = AsTools::Rand( (int)Gap_Height - 1);
		AsTools::Rect(hdc, X_Pos + x, y_offset + y, 1, 1, AsConfig::White_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Act_For_Open(bool short_opening, bool &correct_pos)
{
	correct_pos = false;

	double max_gap_height;
	double gap_height_step;
	int opening_timeout;

	if (short_opening)
	{
		max_gap_height = Max_Gap_Short_Height;
		gap_height_step = Gap_Height_Short_Step;
		opening_timeout = Short_Opening_Timeout;
	}
	else
	{
		max_gap_height = Max_Gap_Long_Height;
		gap_height_step = Gap_Height_Long_Step;
		opening_timeout = Long_Opening_Timeout;
	}

	switch (Gate_Transformation)
	{
	case EGate_Transformation::Init:
		if (Gap_Height < max_gap_height)
		{
			Gap_Height += gap_height_step;
		}
		else
		{
			Gap_Height = max_gap_height;
			Gate_Transformation = EGate_Transformation::Active;
			Gate_Close_Tick = AsConfig::Current_Timer_Tick + opening_timeout;
		}	
		if (! short_opening)
			correct_pos = true;

		return true;

	case EGate_Transformation::Active:
		if (AsConfig::Current_Timer_Tick >= Gate_Close_Tick)
			Gate_Transformation = EGate_Transformation::Finalize;

		break;

	case EGate_Transformation::Finalize:
		if (Gap_Height > 0.0)
		{
			Gap_Height -= gap_height_step;
		}
		else
		{
			Gap_Height = 0.0;
			Gate_Transformation = EGate_Transformation::Unknown;
			Gate_State = EGate_State::Closed;
		}

		if (! short_opening)
			correct_pos = true;

		return true;
	}	

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Redraw_Gate()
{
	--Rect.top;
	++Rect.bottom;
	AsTools::Invalidate_Rect(Rect);
	++Rect.top;
	--Rect.bottom;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Rect) )
		return;

	AsTools::Rect(hdc, Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Open_Gate(bool short_open)
{
	if (Gate_State != EGate_State::Closed)
		return;

	if (short_open)
		Gate_State = EGate_State::Short_Open;
	else
		Gate_State = EGate_State::Long_Open;

	Gate_Transformation = EGate_Transformation::Init;
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Opened() const
{
	if (Gate_State == EGate_State::Short_Open or Gate_State == EGate_State::Long_Open)
		if (Gate_Transformation == EGate_Transformation::Active)
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Get_Y_Limits(int &gate_top_y, int &gate_low_y) const
{
	gate_top_y = Rect.top;
	gate_low_y = Rect.bottom;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Get_Pos(int &x_pos, int &y_pos)
{
	x_pos = X_Pos;
	y_pos = (int)Origin_Y_Pos;
}
//------------------------------------------------------------------------------------------------------------