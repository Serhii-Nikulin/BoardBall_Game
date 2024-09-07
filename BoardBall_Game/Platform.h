#pragma once
#include "Falling_Letter.h"
#include "Ball_Set.h"

//------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
	EPS_Missing, 
	EPS_Ready, 
	EPS_Normal,
	EPS_Meltdown, 
	EPS_Roll_In, 
	EPS_Expand_Roll_In,
	EPS_Adhesive
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Substate_Meltdown
{
	EPSM_Unknown,
	EPSM_Init,
	EPSM_Active
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Substate_Adhesive
{
	EPSA_Unknown,
	EPSA_Init,
	EPSA_Active,
	EPSA_Finalize
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Moving_State
{
	EPMS_Stop,
	EPMS_Moving_Left,
	EPMS_Moving_Right
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
	AsPlatform();
	void Init(AsBall_Set *ball_set);

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw(bool update_rect = true);
	void Set_State(EPlatform_State platform_state);
	EPlatform_State Get_State();
	void Move(bool to_left, bool key_down);
	bool Hit_By(AFalling_Letter *falling_letter);
	void On_Space_Key(bool key_down);

	int Width;
	int Inner_Width;
	double X_Pos;

private:
	void Act_For_Meltdown_State();
	void Act_For_Adhesive_State();
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Get_Normal_Platform_Image(HDC hdc);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	bool Get_Platform_Image_Storke_Color(int x, int y, int &stroke_len, const AColor **color);
	void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
	void Draw_Expandig_Roll_In_State(HDC hdc, RECT paint_area);
	void Draw_Adhesive_State(HDC hdc, RECT &paint_area);
	void Draw_Adhesive_Spot(HDC hdc, int x_offset, int width, int heigth);
	bool Reflect_On_Circle(double next_x_pos, double next_y_pos, ABall *ball, double x_offset = 0);

	AsBall_Set *Ball_Set;

	EPlatform_State Platform_State;
	EPlatform_Substate_Meltdown Platform_Substate_Meltdown;
	EPlatform_Substate_Adhesive Platform_Substate_Adhesive;
	EPlatform_Moving_State Platform_Moving_State;

	AColor Platform_Inner_Color, Platform_Circle_Color, Highlight_Color;

	RECT Prev_Platform_Rect, Platform_Rect;

	int Rolling_Step;
	bool Left_Key_Down, Right_Key_Down;

	static const int Circle_Size = 7;
	static const int Roll_In_Platform_End_X_Pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset) / 2 - Circle_Size / 2;
	static const int Rolling_Platform_Speed = 3;

	static const int Max_Rolling_Step = 16;
	static const int Inner_Height = 5;
	static const int Height = 7;
	static const int Meltdown_Speed = 4;
	static const int Normal_Inner_Width = 20;

	static const int Normal_Width = 28 * AsConfig::Global_Scale;
	static int Meltdown_Platform_Y_Pos[Normal_Width];
	static const int X_Step = 2 * AsConfig::Global_Scale;
	double Speed;
	int Normal_Platform_Image_Width;
	int Normal_Platform_Image_Height;
	int* Normal_Platform_Image;

	double Adhesive_Spot_Height_Ratio;
	static const double Step_Adhesive_Spot_Height_Ratio;
	static const double Min_Adhesive_Spot_Height_Ratio;
	static const double Max_Adhesive_Spot_Height_Ratio;
};
//------------------------------------------------------------------------------------------------------------