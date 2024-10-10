#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Draw(HDC HDC, RECT &paint_area) = 0;
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AMover
{
public:
	virtual ~AMover();
	virtual void Begin_Movement() = 0;
	virtual void Finish_Movement() = 0;
	virtual void Shift_Per_Step(double max_speed) = 0;
	virtual double Get_Speed() = 0;

};
//------------------------------------------------------------------------------------------------------------
class ABall;
//------------------------------------------------------------------------------------------------------------
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);

	bool Hit_Circle_On_Line(double next_pos, double eval_dist, double radius, double min_value, double max_value);
};
//------------------------------------------------------------------------------------------------------------
class AHit_Checker_List
{
public:
	AHit_Checker_List();
	void Add_Hit_Checker(AHit_Checker *hit_checker);
	bool Check_Hit(double x_pos, double y_pos);
	bool Check_Hit(double x_pos, double y_pos, ABall *ball);

private:
	static const int Hit_Checkers_Count = 3;
	int Counter_Hit_Checker;
	AHit_Checker *Hit_Checkers[Hit_Checkers_Count];
};
//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor();
	AColor(unsigned char r, unsigned char g, unsigned char b);
	AColor(const AColor& color, int pen_size);
	AColor(const AColor &pen_color, int pen_size, const AColor &brush_color);

	unsigned char R, G, B;

	int Get_RGB() const;
	void Select(HDC hdc) const;
	void Select_Pen(HDC hdc) const;
	HBRUSH Get_Brush() const;

private:
	HPEN Pen;
	HBRUSH Brush;
};
//------------------------------------------------------------------------------------------------------------
class AGame_Object: public AMover, public AGraphics_Object
{
};
//------------------------------------------------------------------------------------------------------------
class AsGame_Objects_Set: public AGame_Object
{
public:
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Shift_Per_Step(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

protected:
	virtual bool Get_Next_Game_Object(int &index, AGame_Object **game_obj) = 0;
};
//------------------------------------------------------------------------------------------------------------