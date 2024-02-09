#include "Engine.h"


//------------------------------------------------------------------------------------------------------------
ABall::ABall():
	Ball_Pen{}, Ball_Brush{}, Ball_Rect{}, Prev_Ball_Rect{},
	Ball_X_Pos(103 - 28 / 2 + (28 - Ball_Size) / 2), Ball_Y_Pos(AsPlatform::Y_Pos + 1 - Ball_Size), Ball_Speed(AsConfig::Global_Scale), Ball_Direction(M_PI - M_PI_4)
{
	//Ball_X_Pos(X_Pos + (Width - Ball_Size) / 2), Ball_Y_Pos(Y_Pos + 1 - Ball_Size), Ball_Speed(AsConfig::Global_Scale), Ball_Direction(M_PI - M_PI_4)
}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
	AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);

	Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
	Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
		return;

	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
	Prev_Ball_Rect = Ball_Rect;

	int next_x_pos = Ball_X_Pos + int(Ball_Speed * cos(Ball_Direction));
	int next_y_pos = Ball_Y_Pos - int(Ball_Speed * sin(Ball_Direction));
	int max_x_pos = AsBorder::Max_X_Pos - Ball_Size;
	int max_y_pos = AsBorder::Max_Y_Pos - Ball_Size;
	int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

	if (next_x_pos < AsBorder::Border_X_Offset)
	{
		next_x_pos = AsBorder::Border_X_Offset + (AsBorder::Border_X_Offset - next_x_pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos < AsBorder::Border_Y_Offset)
	{
		next_y_pos = AsBorder::Border_Y_Offset + (AsBorder::Border_Y_Offset - next_y_pos);
		Ball_Direction = -Ball_Direction;
	}

	if (next_x_pos > max_x_pos)
	{
		Ball_X_Pos = AsBorder::Max_X_Pos - Ball_Size;
		//next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
		Ball_Direction = -Ball_Direction + M_PI;
	}

	if (next_y_pos > max_y_pos)
	{
		Ball_Y_Pos = AsBorder::Max_Y_Pos - Ball_Size;
		//next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
		Ball_Direction = -Ball_Direction;
	}

	if (next_y_pos > platform_y_pos)
		if (next_x_pos >= platform->X_Pos - Ball_Size
			and 
			next_x_pos <= platform->X_Pos + platform->Width)
		{
			Ball_Direction = -Ball_Direction;
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
		}

	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
	Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, TRUE);
	InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform():
	Inner_Width(20), Width(28), X_Pos(103 - Width / 2), X_Step(2 * AsConfig::Global_Scale),
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
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
	int x = X_Pos;
	int y = Y_Pos;

	RECT intersection_rect{};
	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

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
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
		(x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale,
		x * AsConfig::Global_Scale + 8, y * AsConfig::Global_Scale,
		x * AsConfig::Global_Scale, y * AsConfig::Global_Scale + 8);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw(AsEngine* engine)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
	Platform_Rect.top = Y_Pos * AsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * AsConfig::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, TRUE);
	InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Hwnd{}
{}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	Hwnd = hwnd;

	Ball.Init();
	Level.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw(this);
	SetTimer(Hwnd, Timer_ID, 1000 / 20, NULL);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect{};

	Level.Draw(hdc, paint_area);

	Platform.Draw(hdc, paint_area);

	Ball.Draw(hdc, paint_area);

	Border.Draw(hdc, paint_area);

	/*int i;
	for (i = 0; i < 16; ++i)
	{
		Level.Draw_Brick_Letter(hdc, (20 + i * ALevel::Cell_Width) * Global_Scale, 100, EBT_Blue, ELT_O, i);
		Level.Draw_Brick_Letter(hdc, (20 + i * ALevel::Cell_Width) * Global_Scale, 100 + 50, EBT_Red, ELT_O, i);
	}*/
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos < AsBorder::Border_X_Offset)
			Platform.X_Pos = AsBorder::Border_X_Offset;

		Platform.Redraw(this);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos > AsBorder::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = AsBorder::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw(this);
		break;

	case EKT_Space:

		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	Ball.Move(this, &Level, &Platform);
	return 0;
}
//------------------------------------------------------------------------------------------------------------
