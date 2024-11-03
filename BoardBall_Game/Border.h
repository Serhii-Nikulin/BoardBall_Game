#pragma once
//#include "Config.h"
#include "Gate.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGame_Object
{
public:
	~AsBorder();
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();
	void Open_Gate(int gate_index, bool short_open);
	int Long_Open_Gate();
	bool Is_Gate_Opened(int gate_index);
	bool Is_Gate_Closed(int gate_index);
	void Get_Gate_Pos(int gate_index, int &x_pos, int &y_pos);


private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, RECT &paint_area);
	void Draw_Floor(HDC hdc, RECT &paint_area) const;
	RECT Floor_Rect;

	AGate *Gates[AsConfig::Gates_Count];
};
//------------------------------------------------------------------------------------------------------------