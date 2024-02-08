#include "Engine.h"

char Level_01[AsEngine::Level_Height][AsEngine::Level_Width] = {
//  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//1
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//2
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//3
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//4
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//5
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,//6
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//7
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,//8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//11
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//12
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//13
};
//------------------------------------------------------------------------------------------------------------
ABall::ABall():
	Ball_X_Pos(103 - 28 / 2 + (28 - Ball_Size) / 2), Ball_Y_Pos(AsEngine::Platform_Y_Pos + 1 - Ball_Size), Ball_Speed(AsEngine::Global_Scale), Ball_Direction(M_PI - M_PI_4)
{
	//Ball_X_Pos(Platform_X_Pos + (Platform_Width - Ball_Size) / 2), Ball_Y_Pos(Platform_Y_Pos + 1 - Ball_Size), Ball_Speed(AsEngine::Global_Scale), Ball_Direction(M_PI - M_PI_4)
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw_Ball(HDC hdc, RECT &paint_area)
{
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move_Ball(AsEngine *engine)
{
	Prev_Ball_Rect = Ball_Rect;

	int next_x_pos = Ball_X_Pos + int(Ball_Speed * cos(Ball_Direction));
	int next_y_pos = Ball_Y_Pos - int(Ball_Speed * sin(Ball_Direction));
	int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
	int max_y_pos = AsEngine::Max_Y_Pos - Ball_Size;
	int platform_y_pos = AsEngine::Platform_Y_Pos - Ball_Size;

	if (next_x_pos < AsEngine::Border_X_Offset)
	{
		next_x_pos = AsEngine::Border_X_Offset + (AsEngine::Border_X_Offset - next_x_pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos < AsEngine::Border_Y_Offset)
	{
		next_y_pos = AsEngine::Border_Y_Offset + (AsEngine::Border_Y_Offset - next_y_pos);
		Ball_Direction = -Ball_Direction;
	}

	if (next_x_pos > max_x_pos)
	{
		Ball_X_Pos = AsEngine::Max_X_Pos - Ball_Size;
		//next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
		Ball_Direction = -Ball_Direction + M_PI;
	}

	if (next_y_pos > max_y_pos)
	{
		Ball_Y_Pos = AsEngine::Max_Y_Pos - Ball_Size;
		//next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
		Ball_Direction = -Ball_Direction;
	}

	if (next_y_pos > platform_y_pos)
		if (next_x_pos >= engine->Platform_X_Pos - Ball_Size
			and 
			next_x_pos <= engine->Platform_X_Pos + engine->Platform_Width)
		{
			Ball_Direction = -Ball_Direction;
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
		}

	engine->Check_Level_Brick_Hit(next_y_pos);

	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Ball_Rect.left = Ball_X_Pos * AsEngine::Global_Scale;
	Ball_Rect.top = Ball_Y_Pos * AsEngine::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * AsEngine::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsEngine::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, TRUE);
	InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine():
	Inner_Width(20), Platform_Width(28), Platform_X_Pos(103 - Platform_Width / 2), Platform_X_Step(2 * Global_Scale)
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	Hwnd = hwnd;

	Highlight_Pen = Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	Create_Pen_Brush(63, 72, 204, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(237, 38, 36, Brick_Red_Pen, Brick_Red_Brush);
	Create_Pen_Brush(63, 72, 204, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(237, 38, 36, Platform_Inner_Pen, Platform_Inner_Brush);
	Create_Pen_Brush(0, 0, 0, BG_Pen, BG_Brush);
	Create_Pen_Brush(255, 255, 255, Ball.Ball_Pen,Ball. Ball_Brush);

	Create_Pen_Brush(63, 72, 204, Border_Blue_Pen, Border_Blue_Brush);
	Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);

	Level_Rect.left = Level_X_Offset * Global_Scale;
	Level_Rect.top = Level_Y_Offset * Global_Scale;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Global_Scale;

	Ball.Ball_Rect.left = Ball.Ball_X_Pos * Global_Scale;
	Ball.Ball_Rect.top = Ball.Ball_Y_Pos * Global_Scale;
	Ball.Ball_Rect.right = Ball.Ball_Rect.left + Ball.Ball_Size * Global_Scale;
	Ball.Ball_Rect.bottom = Ball.Ball_Rect.top + Ball.Ball_Size * Global_Scale;

	Redraw_Platform();
	SetTimer(Hwnd, Timer_ID, 1000 / 20, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect{};

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		Draw_Level(hdc);

	if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

	if (IntersectRect(&intersection_rect, &paint_area, &Ball.Ball_Rect))
		Ball.Draw_Ball(hdc, paint_area);

	Draw_Bounds(hdc, paint_area);

	/*int i;
	for (i = 0; i < 16; ++i)
	{
		Draw_Brick_Letter(hdc, (20 + i * Cell_Width) * Global_Scale, 100, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, (20 + i * Cell_Width) * Global_Scale, 100 + 50, EBT_Red, ELT_O, i);
	}*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform_X_Pos -= Platform_X_Step;

		if (Platform_X_Pos < Border_X_Offset)
			Platform_X_Pos = Border_X_Offset;

		Redraw_Platform();
		break;

	case EKT_Right:
		Platform_X_Pos += Platform_X_Step;

		if (Platform_X_Pos > Max_X_Pos - Platform_Width + 1)
			Platform_X_Pos = Max_X_Pos - Platform_Width + 1;

		Redraw_Platform();
		break;

	case EKT_Space:

		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	Ball.Move_Ball(this);
	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Level(HDC hdc)
{
	int i, j;

	for (i = 0; i < Level_Height; ++i)
		for (j = 0; j < Level_Width; ++j)
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + Cell_Height * i, static_cast<EBrick_Type>(Level_01[i][j]));
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_Blue:
		pen = Brick_Blue_Pen;
		brush = Brick_Blue_Brush;
		break;

	case EBT_Red:
		pen = Brick_Red_Pen;
		brush = Brick_Red_Brush;
		break;

	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Platform(HDC hdc, int x, int y)
{
	//draw side parts
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + Platform_Width - Circle_Size) * Global_Scale, y * Global_Scale, (x + Platform_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);

	//draw inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width) * Global_Scale, (y + 1 + Inner_Height) * Global_Scale, Inner_Height * Global_Scale, Inner_Height * Global_Scale);

	//draw highlight
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale,
		(x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
		x * Global_Scale + 8, y * Global_Scale,
		x * Global_Scale, y * Global_Scale + 8);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Redraw_Platform()
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = Platform_X_Pos * Global_Scale;
	Platform_Rect.top = Platform_Y_Pos * Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

	InvalidateRect(Hwnd, &Prev_Platform_Rect, TRUE);
	InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Check_Level_Brick_Hit(int &next_y_pos)
{
	int i, j;
	int brick_y_pos = AsEngine::Level_Y_Offset + (Level_Height - 1) * Cell_Height + Brick_Height;

	for (i = Level_Height - 1; i >= 0; --i)
	{
		for (j = 0; j < Level_Width; ++j)
		{
			if (Level_01[i][j] == 0)
				continue;

			if (brick_y_pos > next_y_pos)
			{
				Ball.Ball_Direction = -Ball.Ball_Direction;
				next_y_pos = brick_y_pos + (brick_y_pos - next_y_pos);
				break;
			}
		}
		brick_y_pos -= Cell_Height;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Bounds(HDC hdc, RECT &paint_area)
{
	int i;

	for (i = 0; i < 50; ++i)
		Draw_Border(hdc, 3 + i * 4, 0, true);//top horizontal part

	for (i = 0; i < 50; ++i)
		Draw_Border(hdc, 2, 1 + i * 4, false);//left part

	for (i = 0; i < 50; ++i)
		Draw_Border(hdc, 201, 1 + i * 4, false);//right part
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (top_border)
		Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 1) * Global_Scale);
	else
		Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 1) * Global_Scale, (y + 4) * Global_Scale);

	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (top_border)
		Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale, (x + 4) * Global_Scale, (y + 1 + 3) * Global_Scale);
	else
		Rectangle(hdc, (x + 1) * Global_Scale, y * Global_Scale, (x + 1 + 3) * Global_Scale, (y + 4) * Global_Scale);

	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);

	if (top_border)
		Rectangle(hdc, (x + 2) * Global_Scale, (y + 2) * Global_Scale, (x + 3) * Global_Scale, (y + 3) * Global_Scale);
	else
		Rectangle(hdc, (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 3) * Global_Scale, (y + 2) * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	bool switch_color;
	double offset;
	double rotation_angle;
	int back_part_offset;
	int brick_half_height = Brick_Height * Global_Scale / 2;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xform, prev_xform;

	if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
		return;

	rotation_step %= 16;

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI / 16 * rotation_step;
	else
		rotation_angle = 2.0 * M_PI / 16 * (8 - rotation_step);

	if (rotation_step > 4 and rotation_step <= 12)
	{
		if (brick_type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
	}
	else
	{
		if (brick_type == EBT_Blue)
			switch_color = false;
		else
			switch_color = true;
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (rotation_step == 4 or rotation_step == 12)
	{
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

		//Front side
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);
		GetWorldTransform(hdc, &prev_xform);

		xform.eM11 = (FLOAT)1; xform.eM12 = (FLOAT)0;
		xform.eM21 = (FLOAT)0; xform.eM22 = (FLOAT)cos(rotation_angle);
		xform.eDx = (FLOAT)x;
		xform.eDy = (FLOAT)y + brick_half_height;

		SetWorldTransform(hdc, &xform);

		offset = 3.0 * (1.0f - fabs(cos(rotation_angle))) * Global_Scale;
		back_part_offset = (int)round(offset);

		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, +brick_half_height - back_part_offset);

		//Front side
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -(int)brick_half_height, Brick_Width * Global_Scale, +(int)brick_half_height);

		if (rotation_step > 4 and rotation_step < 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc,
					(Brick_Width - (Brick_Height - 2)) * Global_Scale / 2,
					-(Brick_Height - 2) * Global_Scale / 2,
					(Brick_Width + (Brick_Height - 2)) * Global_Scale / 2,
					+(Brick_Height - 2) * Global_Scale / 2);
			}
		}

		SetWorldTransform(hdc, &prev_xform);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
	if (is_switch_color)
	{
		front_pen = Brick_Red_Pen;
		front_brush = Brick_Red_Brush;
		back_pen = Brick_Blue_Pen;
		back_brush = Brick_Blue_Brush;
	}
	else
	{
		front_pen = Brick_Blue_Pen;
		front_brush = Brick_Blue_Brush;
		back_pen = Brick_Red_Pen;
		back_brush = Brick_Red_Brush;
	}
}
//------------------------------------------------------------------------------------------------------------