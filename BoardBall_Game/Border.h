#pragma once
//#include "Config.h"
#include "Ball.h"
#include "Gate.h"

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
	void Open_Gate(int gate_index, bool short_open);
	bool Is_Gate_Opened(int gate_index);

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area);
	void Draw_Floor(HDC hdc, RECT &paint_area) const;
	RECT Floor_Rect;

	AGate *Gates[AsConfig::Gates_Count];
};
//------------------------------------------------------------------------------------------------------------