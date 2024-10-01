#pragma once
#include "Falling_Letter.h"
#include "Ball_Set.h"// + Ball.h
#include "Laser_Beam_Set.h"// + Laser_Beam.h
#include "Platform_State.h"

//------------------------------------------------------------------------------------------------------------
class AsPlatform_Adhesive
{
public:
	AsPlatform_Adhesive(AsPlatform_State &platform_state);
	bool Act(AsBall_Set *ball_set, EPlatform_State &next_state);

	void Draw_State(HDC hdc, double x_pos);
	void Draw_Adhesive_Spot(HDC hdc, double x_pos, int x_offset, int width, int heigth);
	void Reset();

private:
	double Adhesive_Spot_Height_Ratio;

	static const double Step_Adhesive_Spot_Height_Ratio;
	static const double Min_Adhesive_Spot_Height_Ratio;
	static const double Max_Adhesive_Spot_Height_Ratio;

	AsPlatform_State *Platform_State;
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_Expanding
{
public:
	~AsPlatform_Expanding();
	AsPlatform_Expanding(AsPlatform_State &platform_state);

	bool Act(double &x_pos, double &current_width, EPlatform_State &next_state);
	void Draw_State(HDC hdc, double x, double &current_width, RECT &Platform_Rect);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Expanding_Truss(HDC hdc, double x, int y, double ratio);
	void Reset(double &width);
	void Init(AColor &inner_color, AColor &circle_color, AColor &Higlight_Color);

private:
	static const double Step_Expanding_Width;
	static const double Min_Expanding_Width;
	static const double Max_Expanding_Width;

	AsPlatform_State *Platform_State;

	AColor *Inner_Color, *Circle_Color, *Highlight_Color;
	AColor *Truss_Expanding_Color;//UNO

};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_Laser
{
public:
	~AsPlatform_Laser();
	AsPlatform_Laser(AsPlatform_State &platform_state);
	void Init(AsLaser_Beam_Set *laser_beam_set, AColor &inner_color, AColor &circle_color, AColor &white_color);
	bool Act(EPlatform_State &next_state, double x_pos);
	void Draw_State(HDC hdc, double x_pos);
	void Reset();
	void Fire(bool fire_on);
private:
	void Draw_Laser_Inner_Part(HDC hdc, double x_pos);
	void Draw_Laser_Wing(HDC hdc, bool is_left, double x_pos);
	void Draw_Laser_Leg(HDC hdc, bool is_left, double x_pos);
	void Draw_Laser_Cabin(HDC hdc, double x_pos);
	void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
	double Get_Expanding_Value(double start, double end, double ratio);
	double Get_Gun_X_Pos(bool is_left, double platform_x_pos);

	AsPlatform_State *Platform_State;

	AColor *Inner_Color, *Circle_Color, *White_Color;
	AColor *Gun_Color;

	AsLaser_Beam_Set *Laser_Beam_Set;// UNO

	bool Enable_Laser_Firing;
	int Laser_Transformation_Step;

	int Last_Laser_Shot_Tick;

	static const int Laser_Shot_Timeout = AsConfig::FPS / 2;
	static const int Max_Laser_Transformation_Step = 30;
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
	AsPlatform();
	void Init(AsLaser_Beam_Set *laser_beam_set, AsBall_Set *ball_set);

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
	void Set_State(EPlatform_Substate_Regular new_regular_state);
	EPlatform_State Get_State();
	void Move(bool to_left, bool key_down);
	bool Has_State(EPlatform_Substate_Regular regular_state);
	bool Hit_By(AFalling_Letter *falling_letter) const;
	void On_Space_Key(bool key_down);

	double Width;
	int Inner_Width;
	double X_Pos;

	static const int Normal_Width = 28;
	static const int Expanding_Platform_Inner_Width = 12;
	static const int Circle_Size = 7;
	static const int Inner_Height = 5;

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

	static const int Roll_In_Platform_End_X_Pos = AsConfig::Border_X_Offset + (AsConfig::Max_X_Pos - AsConfig::Border_X_Offset) / 2 - Circle_Size / 2;
	static const int Rolling_Platform_Speed = 3;

	static const int Max_Rolling_Step = 16;
	static const int Height = 7;
	static const int Meltdown_Speed = 4;
	static const int Normal_Inner_Width = 20;

	static int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];
	static const int X_Step = 2 * AsConfig::Global_Scale;
	double Speed;
	int Normal_Platform_Image_Width;
	int Normal_Platform_Image_Height;
	int* Normal_Platform_Image;

	int Last_Redraw_Timer_Tick;
};
//------------------------------------------------------------------------------------------------------------