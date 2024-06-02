#pragma once
#include "Active_Brick.h"

enum ELetter_Type {
	ELT_None, ELT_O
};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);

	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;
	int Rotation_Step;
	int X, Y;
	bool Finished;
	bool Got_Hit;

public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();
	
	void Get_Letter_Cell(RECT &rect);
	void Finalize();

	RECT Letter_Cell, Prev_Letter_Cell;
};