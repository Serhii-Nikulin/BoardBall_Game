#pragma once
#include "Active_Brick.h"

enum ELetter_Type {
	ELT_None,

	ELT_O, // To cancel
	ELT_I, // Inversion
	ELT_C, // Speed
	ELT_M, // To generate the monster
	ELT_G, // Life
	ELT_K, // To capture the ball by platform
	ELT_W, // To wide
	ELT_P, // Floor
	ELT_L, // Lazer
	ELT_T, // three
	ELT_Plus // pass to the next level
};
//------------------------------------------------------------------------------------------------------------
enum EFalling_Letter_State{
	EFLS_Normal, EFLS_Finalizing, EFLS_Finished
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

	EFalling_Letter_State Falling_Letter_State;

	static const int Max_Rotation_Step = 16;

public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();
	
	void Get_Letter_Cell(RECT &rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);

	RECT Letter_Cell, Prev_Letter_Cell;
};