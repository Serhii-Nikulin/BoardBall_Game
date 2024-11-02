#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//------------------------------------------------------------------------------------------------------------
enum class EBall_State: unsigned char
{
	Disabled,

	Normal, 
	Lost, 
	On_Platform, 
	On_Parachute, 
	Off_Parachute, 
	Teleporting
};
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
class ABall_Object
{
public:
	virtual void Set_Direction(double speed) = 0;
	virtual double Get_Direction() = 0;

	virtual EBall_State Get_State() = 0;
	static const int Start_Ball_Position_On_Platform = 104;
	static const int Platform_Y_Pos = 185;
	static const int Ball_Radius = 2;

	/*virtual void Set_State(EBall_State new_state, double x_pos = AsConfig::Start_Ball_Position_On_Platform, double y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Radius + 1 + 1.0 / AsConfig::Global_Scale, double direction = M_PI_4) = 0;	*/
	
	virtual void Set_State(EBall_State new_state, double x_pos = Start_Ball_Position_On_Platform, double y_pos = Platform_Y_Pos - Ball_Radius + 1 + 1.0 / 3, double direction = M_PI_4) = 0;
	virtual void Reflect(bool from_horizontal) = 0;
	virtual void Draw_Teleporting(HDC hdc, int step) = 0;
	virtual void Set_On_Parachute(int level_x, int level_y) = 0;
	virtual void Get_Center(double &x_pos, double &y_pos) = 0;
	virtual bool Is_Moving_Up() = 0;
	virtual bool Is_Moving_Left() = 0;

	virtual double Get_Speed() = 0;
	virtual void Set_Speed(double speed) = 0;
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
class AGame_Objects_Set: public AGame_Object
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