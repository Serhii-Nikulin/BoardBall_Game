#pragma once
#include "Falling_Letter.h"
#include "Platform_Expanding.h"// + Platform_State.h
#include "Platform_Adhesive.h"// + Ball_Set.h + Ball.h
#include "Platform_Laser.h"// + Laser_Beam_Set.h + Laser_Beam.h

//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker, public AGame_Object
{
public:
	AsPlatform();
	void Init(AsLaser_Beam_Set *laser_beam_set, AsBall_Set *ball_set);

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);
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
	void Set_State(EPlatform_Substate_Regular new_regular_state);
	EPlatform_State Get_State();
	void Move(bool to_left, bool key_down);
	bool Has_State(EPlatform_Substate_Regular regular_state);
	bool Hit_By(AFalling_Letter *falling_letter) const;
	void On_Space_Key(bool key_down);

	static AHit_Checker_List Hit_Checker_List;

	double Width;
	int Inner_Width;
	double X_Pos;

private:
	bool Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation &transformation_state);
	void Act_For_Meltdown_State();
	void Act_For_Rolling_State();
	void Draw_Rolling_State(HDC hdc, RECT &paint_area);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Get_Normal_Platform_Image(HDC hdc);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	bool Get_Platform_Image_Storke_Color(int x, int y, int &stroke_len, const AColor **color);
	void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
	void Draw_Expandig_Roll_In_State(HDC hdc, RECT paint_area);
	bool Reflect_On_Circle(double next_x_pos, double next_y_pos, ABall *ball, double x_offset = 0);

	AsPlatform_Adhesive Platform_Adhesive;
	AsPlatform_Expanding Platform_Expanding;
	AsPlatform_Laser Platform_Laser;

	AsBall_Set *Ball_Set;

	AsPlatform_State Platform_State;

	AColor Platform_Inner_Color, Platform_Circle_Color, Highlight_Color, Truss_Expanding_Color, Gun_Color;

	RECT Prev_Platform_Rect, Platform_Rect;

	int Rolling_Step;
	bool Left_Key_Down, Right_Key_Down;

	static const int Roll_In_Platform_End_X_Pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset) / 2 - AsConfig::Platform_Circle_Size / 2;
	static const int Rolling_Platform_Speed = 3;

	static const int Max_Rolling_Step = 16;
	static const int Height = 7;
	static const int Meltdown_Speed = 4;
	static const int Normal_Inner_Width = 20;

	static int Meltdown_Platform_Y_Pos[AsConfig::Platform_Normal_Width * AsConfig::Global_Scale];
	static const int X_Step = 2 * AsConfig::Global_Scale;
	double Speed;
	int Normal_Platform_Image_Width;
	int Normal_Platform_Image_Height;
	int* Normal_Platform_Image;

	int Last_Redraw_Timer_Tick;
};
//------------------------------------------------------------------------------------------------------------