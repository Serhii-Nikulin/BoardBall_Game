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
	Create_Pen_Brush(63, 72, 204, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(237, 38, 36, Brick_Red_Pen, Brick_Red_Brush);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	HPEN pen;
	HBRUSH brush;
	HPEN blue_pen = CreatePen(PS_SOLID, 0, RGB(63, 72, 204));
	HBRUSH blue_brush = CreateSolidBrush(RGB(63, 72, 204));
	HPEN red_pen = CreatePen(PS_SOLID, 0, RGB(237, 38, 36));
	HBRUSH red_brush = CreateSolidBrush(RGB(237, 38, 36));

	switch (brick_type)
	{
	case EBT_Blue:
		pen = blue_pen;
		brush = blue_brush;
		break;

	case EBT_Red:
		pen = red_pen;
		brush = red_brush;
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
void Draw_Frame(HDC hdc)
{
	Draw_Level(hdc);
}
//------------------------------------------------------------------------------------------------------------