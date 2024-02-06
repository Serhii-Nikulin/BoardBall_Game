#include "Engine.h"

enum EBrick_Type {
	EBT_None, EBT_Red, EBT_Blue
};

HPEN Brick_Blue_Pen;
HBRUSH Brick_Blue_Brush;
HPEN Brick_Red_Pen;
HBRUSH Brick_Red_Brush;

const int Global_Scale = 3;
const int Brick_Height = 7;
const int Brick_Width = 15;
const int Cell_Height = 8;
const int Cell_Width = 16;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Level_Height = 14;
const int Level_Width = 12;

const int Inner_Width = 20;
const int Inner_Height = 5;
const int Circle_Size = 7;
const int Full_Platform_Width = 28;
HPEN Platform_Inner_Pen;
HBRUSH Platform_Inner_Brush;
HPEN Platform_Circle_Pen;
HBRUSH Platform_Circle_Brush;

HPEN Highlight_Pen, Letter_Pen;

char Level_01[Level_Height][Level_Width] = {
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
void Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void Init()
{
	Highlight_Pen = Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

	Create_Pen_Brush(63, 72, 204, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(237, 38, 36, Brick_Red_Pen, Brick_Red_Brush);
	Create_Pen_Brush(63, 72, 204, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(237, 38, 36, Platform_Inner_Pen, Platform_Inner_Brush);

}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
void Draw_Level(HDC hdc)
{
	int i, j;

	for (i = 0; i < Level_Height; ++i)
		for (j = 0; j < Level_Width; ++j)
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + Cell_Height * i, static_cast<EBrick_Type>(Level_01[i][j]));
}
//------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
{
	//draw side parts
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + Full_Platform_Width - Circle_Size) * Global_Scale, y * Global_Scale, (x + Full_Platform_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);

	//draw inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width) * Global_Scale, (y + 1 + Inner_Height) * Global_Scale, Inner_Height * Global_Scale, Inner_Height * Global_Scale);

	//draw highlight
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale,
		(x + Circle_Size - 1) * Global_Scale,  (y + Circle_Size - 1) * Global_Scale,
		x * Global_Scale + 8, y * Global_Scale,
		x * Global_Scale, y * Global_Scale + 8);
}
//------------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
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
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, int rotation_step)
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
			SelectObject(hdc, Letter_Pen);

			Ellipse(hdc, 
				(Brick_Width - (Brick_Height - 2)) * Global_Scale / 2,
				-(Brick_Height - 2) * Global_Scale / 2,
				(Brick_Width + (Brick_Height - 2)) * Global_Scale / 2,
				+(Brick_Height - 2) * Global_Scale / 2);
		}
		
		SetWorldTransform(hdc, &prev_xform);
	}
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{
	int i;
	int x = 95;
	int y = 185;
	Draw_Level(hdc);
	Draw_Platform(hdc, x, y);
	
	/*for (i = 0; i < 16; ++i)
	{
		Draw_Brick_Letter(hdc, (20 + i * Cell_Width) * Global_Scale, 100, EBT_Blue, i);
		Draw_Brick_Letter(hdc, (20 + i * Cell_Width) * Global_Scale, 100 + 50, EBT_Red, i);
	}*/
		
}
//------------------------------------------------------------------------------------------------------------