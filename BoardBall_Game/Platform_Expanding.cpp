#include "Platform_Expanding.h"

//AsPlatform_Expanding
const double AsPlatform_Expanding::Step_Expanding_Width = 0.5;
const double AsPlatform_Expanding::Min_Expanding_Width = 28.0;
const double AsPlatform_Expanding::Max_Expanding_Width = 40.0;
//------------------------------------------------------------------------------------------------------------
AsPlatform_Expanding::~AsPlatform_Expanding()
{
	Inner_Color = 0;
	Circle_Color = 0;
	Truss_Expanding_Color = 0;

	delete Truss_Expanding_Color;
	Truss_Expanding_Color = 0;
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_Expanding::AsPlatform_Expanding(AsPlatform_State &platform_state)
	: Platform_State(&platform_state), Inner_Color(0), Circle_Color(0), Highlight_Color(0), Truss_Expanding_Color(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Expanding::Act(double &x_pos, double &current_width, EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;
	double offset = Step_Expanding_Width / 2.0;

	switch (Platform_State->Expanding)
	{
	case EPlatform_Transformation::Init:
		if (current_width < Max_Expanding_Width)
		{
			current_width += Step_Expanding_Width;

			if (x_pos - offset > AsConfig::Border_X_Offset)
			{
				if (x_pos < AsConfig::Max_X_Pos - current_width + 1.0)
					x_pos -= offset;
				else
					x_pos -= Step_Expanding_Width;
			}
		}
		else
		{
			current_width = Max_Expanding_Width;
			Platform_State->Expanding = EPlatform_Transformation::Active;
		}	

		return true;

	case EPlatform_Transformation::Finalize:
		if (current_width > Min_Expanding_Width)
		{
			current_width -= Step_Expanding_Width;
			x_pos += offset;
		}
		else
		{
			current_width = Min_Expanding_Width;
			Platform_State->Expanding = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
		}

		return true;
	}	

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_State(HDC hdc, double x, double &current_width, RECT &Platform_Rect)
{
	double d_scale = AsConfig::D_Global_Scale;
	int scale = AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos;
	int ellipse_offset = 4;
	int ellipse_size = 5;
	double middle_pos;
	double ratio;
	int x_offset;
	RECT inner_rect{}, ellipse_rect{};

	middle_pos = x + current_width / 2.0;

	inner_rect.left = int( (middle_pos - AsConfig::Platform_Expanding_Inner_Width / 2.0) * d_scale);
	inner_rect.right = inner_rect.left + AsConfig::Platform_Expanding_Inner_Width * scale;
	inner_rect.top = (y + 1) * scale;
	inner_rect.bottom = inner_rect.top + AsConfig::Platform_Inner_Height * scale - 1;

	//0.Side parts
	Circle_Color->Select(hdc);

	Ellipse(hdc, Platform_Rect.left, Platform_Rect.top, Platform_Rect.left + AsConfig::Platform_Circle_Size * scale - 1, Platform_Rect.bottom - 1);
	Ellipse(hdc, Platform_Rect.right - AsConfig::Platform_Circle_Size * scale, Platform_Rect.top, Platform_Rect.right - 1, Platform_Rect.bottom - 1);

	Draw_Circle_Highlight(hdc, int(x * d_scale), y * scale);

	//1.Left part
	ellipse_rect.top = (y + 1) * scale;
	ellipse_rect.bottom = ellipse_rect.top + ellipse_size * scale - 1;
	ellipse_rect.left = Platform_Rect.left + ellipse_offset * scale;
	ellipse_rect.right = ellipse_rect.left + ellipse_size * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Ellipse(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom);
	Truss_Expanding_Color->Select_Pen(hdc);
	Arc(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom,
		ellipse_rect.left, ellipse_rect.top, ellipse_rect.left, ellipse_rect.bottom);

	//2.Right part
	ellipse_rect.left = Platform_Rect.right - (ellipse_offset + ellipse_size) * scale;
	ellipse_rect.right = ellipse_rect.left + ellipse_size * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Ellipse(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom);
	Truss_Expanding_Color->Select_Pen(hdc);
	Arc(hdc, ellipse_rect.left, ellipse_rect.top, ellipse_rect.right, ellipse_rect.bottom,
		ellipse_rect.right, ellipse_rect.bottom, ellipse_rect.right, ellipse_rect.top);

	//3.Inner part
	Inner_Color->Select(hdc);
	Rectangle(hdc, inner_rect.left, inner_rect.top, inner_rect.right, inner_rect.bottom); // ##
	//AsTools::Rect(hdc, inner_rect, *Inner_Color);
	
	//4.Truss
	ratio = current_width / Max_Expanding_Width;
	Draw_Expanding_Truss(hdc, x, y, ratio);
	x_offset = int( ( (Max_Expanding_Width - AsConfig::Platform_Expanding_Inner_Width - ellipse_offset * 2.0) / 2.0 + AsConfig::Platform_Expanding_Inner_Width) * ratio - 1);
	x += x_offset;
	Draw_Expanding_Truss(hdc, x, y, ratio);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
	Highlight_Color->Select_Pen(hdc);

	Arc(hdc,
		x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
		x + (AsConfig::Platform_Circle_Size - 1) * AsConfig::Global_Scale, y + (AsConfig::Platform_Circle_Size - 1) * AsConfig::Global_Scale,
		x + (AsConfig::Platform_Circle_Size / 2) * AsConfig::Global_Scale, y,
		x, y + (AsConfig::Platform_Circle_Size / 2) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Draw_Expanding_Truss(HDC hdc, double x, int y, double ratio)
{
	int scale = AsConfig::Global_Scale;
	double d_scale = AsConfig::D_Global_Scale;

	Truss_Expanding_Color->Select_Pen(hdc);

	MoveToEx(hdc, int( (x + 5) * scale + 1), (y + 2) * scale - 1, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 5) * scale + 1);
	LineTo(hdc, int( (x + 14 * ratio) * d_scale - 1), (y + 2) * scale - 1);

	MoveToEx(hdc, int( (x + 5) * d_scale + 1), (y + 5) * scale, FALSE);
	LineTo(hdc, int( (x + 10 * ratio) * d_scale - 1), (y + 1) * scale + 1);
	LineTo(hdc, int( (x + 14 * ratio) * d_scale - 1), (y + 5) * scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Reset(double &width)
{
	width = Min_Expanding_Width;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Expanding::Init(AColor &inner_color, AColor &circle_color, AColor &highlight_color)
{
	Inner_Color = &inner_color;
	Circle_Color = &circle_color;
	Highlight_Color = &highlight_color;

	Truss_Expanding_Color = new AColor(*Inner_Color, AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------