#pragma once
//#include "Config.h"
#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AGate: public AGraphics_Object
{
public:
	AGate(int x_pos, int y_pos);
	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

private:
	void Draw_Cup(HDC hdc, bool is_top);
	void Draw_Edge(HDC hdc, int x, int y, bool is_longer);
	int X_Pos, Y_Pos;
	const int Width = 6;
	const int Height = 19;

	RECT Rect;
};
//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGraphics_Object
{
public:
	~AsBorder();
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area);
	void Draw_Floor(HDC hdc, RECT &paint_area);
	RECT Floor_Rect;

	AGate *Gates[AsConfig::Gates_Counter];
};
//------------------------------------------------------------------------------------------------------------