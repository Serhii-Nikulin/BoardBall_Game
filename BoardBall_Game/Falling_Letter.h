#pragma once
#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
enum class ELetter_Type: unsigned char
{
//None,
	O, // To cancel
	M, // To generate the monster
	I, // Inversion
	C, // Speed
	K, // To capture the ball by platform
	W, // To wide

	G, // Life
	T, // three
	L, // Lazer
	P, // Floor
	Plus, // pass to the next level
	Max
};
//------------------------------------------------------------------------------------------------------------
enum class EFalling_Letter_State: unsigned char
{
	Normal, 
	Finalizing, 
	Finished
};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
private:

	void Draw_Brick_Letter(HDC hdc);
	void Set_Brick_Letter_Colors(bool is_switch_color, const AColor **front_color, const AColor **back_color);
	
	const EBrick_Type Brick_Type;
	int Rotation_Step;
	int X, Y;

	EFalling_Letter_State Falling_Letter_State;

	static const int Max_Rotation_Step = 16;
	static const int Letters_Popularity[(char)ELetter_Type::Max];
	static int All_Letters_Popularity;

public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();
	
	void Get_Letter_Cell(RECT &rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);
	void Show_Full_Size();

	static void Init();
	static ELetter_Type Get_Random_Letter_Type();
	RECT Letter_Cell, Prev_Letter_Cell;
	const ELetter_Type Letter_Type;
};
//------------------------------------------------------------------------------------------------------------