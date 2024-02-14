#include "Platform.h"

int AsPlatform::Meltdown_Platform_Y_Pos[Normal_Width];
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform():
	Platform_State(EPS_Normal), Inner_Width(20), Width(28), X_Pos(103 - Width / 2), X_Step(2 * AsConfig::Global_Scale),
	Platform_Inner_Pen{}, Platform_Inner_Brush{}, Platform_Circle_Pen{}, Platform_Circle_Brush{}, Highlight_Pen{}, Prev_Platform_Rect{}, Platform_Rect{}
{}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(63, 72, 204, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(237, 38, 36, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act(HWND hwnd)
{
	if (Platform_State == EPS_Meltdown or Platform_State == EPS_Roll_In)
		Redraw(hwnd);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State platform_state)
{
	int i, len;
	 
	if (Platform_State == platform_state)
		return;

	if (platform_state == EPS_Meltdown)
	{
		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(int);
		for (i = 0; i < len; i++)
		Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
	}

	Platform_State = platform_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
	switch (Platform_State)
	{
	case EPS_Normal:
		Draw_Normal_State(hdc, paint_area);
		break;

	case EPS_Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPS_Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc)
{
	int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;

	SelectObject(hdc, Highlight_Pen);
	Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
		(x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale,
		x * AsConfig::Global_Scale + 8, y * AsConfig::Global_Scale,
		x * AsConfig::Global_Scale, y * AsConfig::Global_Scale + 8);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
	int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;

	RECT intersection_rect{};

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect))
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);
		Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
	{
		//draw side parts
		SelectObject(hdc, Platform_Circle_Pen);
		SelectObject(hdc, Platform_Circle_Brush);
		Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
		Ellipse(hdc, (x + Width - Circle_Size) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Width) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);

		//draw inner part
		SelectObject(hdc, Platform_Inner_Pen);
		SelectObject(hdc, Platform_Inner_Brush);
		RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width) * AsConfig::Global_Scale, (y + 1 + Inner_Height) * AsConfig::Global_Scale, Inner_Height * AsConfig::Global_Scale, Inner_Height * AsConfig::Global_Scale);

		//draw highlight
		Draw_Circle_Highlight(hdc);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect{};
	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect));
	int i, j;
	int area_width, area_height;
	int y_offset;
	int x, y;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(0, 0, 0);

	if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect))
		return;

	area_width = Width * AsConfig::Global_Scale;
	area_height = Height * AsConfig::Global_Scale;
	for (i = 0; i < area_width; i++)
	{
		y_offset = AsConfig::Rand(Meltdown_Speed);

		x = Platform_Rect.left + i;
		for (j = 0; j <= area_height; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - j;
			pixel = GetPixel(hdc, x, y);
			SetPixel(hdc, x, y + y_offset, pixel);
		}
		for (j = 0; j < y_offset; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - area_height + j;
			SetPixel(hdc, x, y, bg_pixel);
		}

		Meltdown_Platform_Y_Pos[i] += y_offset;
	}
	//Platform_State = EPS_Missing;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw(HWND hwnd)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
	Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

	if (Platform_State == EPS_Meltdown)
		Prev_Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;

	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{
	int x = X_Pos * AsConfig::Global_Scale;
	int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
	int roller_size = Circle_Size * AsConfig::Global_Scale;

	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + roller_size, y + roller_size);

	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);
	Rectangle(hdc, x + roller_size / 2 - 1, y, x + roller_size / 2 + 2, y + roller_size);

	Draw_Circle_Highlight(hdc);
}
//------------------------------------------------------------------------------------------------------------
