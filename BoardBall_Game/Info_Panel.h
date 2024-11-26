#pragma once
#include "Falling_Letter.h"

//AsInfo_Panel
//------------------------------------------------------------------------------------------------------------
class AsInfo_Panel: public AGame_Object
{
public:
	~AsInfo_Panel();
	AsInfo_Panel();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	void Show_Extra_Lives(HDC hdc);
	void Draw_String(HDC hdc, const RECT &rect, AString &string, HFONT font);

	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Init();

private:
	HFONT Logo_Font;
	HFONT Name_Font;
	HFONT Score_Font;

	AColor *White_Color_1_px;
	AColor *Black_Color_1_px;
	AColor *Blue_Black;
	AColor *Red_Black;

	void Draw_Extra_Life(HDC hdc, int x, int y);
	void Choose_Font();

	static const int Score_X_Pos = 208;
	static const int Score_Y_Pos = 106;
	static const int Score_Width = 110;
	static const int Score_Height = 92;

	static const int Score_Value_Offset = 22;

	AFalling_Letter Floor_Table;
	AFalling_Letter Lifes_Table;
	AFalling_Letter Monster_Table;

	AString Player_Name;
};
//------------------------------------------------------------------------------------------------------------