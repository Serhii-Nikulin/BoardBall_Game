#pragma once
#include "Ball.h"

enum EBrick_Type 
{
	EBT_None, EBT_Red, EBT_Blue,
	EBT_Unbreakable,
	EBT_Multihit_1, EBT_Multihit_2, EBT_Multihit_3, EBT_Multihit_4,
	EBT_Parachute, 
	EBT_Teleport,
	EBT_Ad, EBT_Invisible
};
//------------------------------------------------------------------------------------------------------------
enum EDirection_Type
{
	EDT_Left,
	EDT_Up,
	EDT_Right,
	EDT_Down
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
class AActive_Brick: public AGraphics_Object
{
public:
	void Get_Level_Pos(int &brick_x, int &brick_y);

protected:
	virtual ~AActive_Brick();
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);
	double Get_Brick_X_Pos(bool is_center);
	double Get_Brick_Y_Pos(bool is_center);
	EBrick_Type Brick_Type;
	RECT Brick_Rect;
	int Level_X, Level_Y;

};
//-----------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
class AActive_Brick_Red_Blue: public AActive_Brick
{
public:
	virtual ~AActive_Brick_Red_Blue();
	AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Draw(HDC hdc, RECT &paint_rect);
	virtual void Act();
	virtual bool Is_Finished();
	static void Setup_Colors();
	static void Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);

private:
	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color);

	int Fade_Step;

	static const int Max_Fade_Step = AsConfig::FPS;
	static AColor Fading_Red_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//------------------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
	~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(EBrick_Type brick_type, int level_x, int level_y);
	
	virtual void Draw(HDC hdc, RECT &paint_rect);
	virtual void Act();
	virtual bool Is_Finished();
	static void Draw_In_Level(HDC hdc, RECT &brick_rect);

	static const int Max_Animation_Step = 15;
	int Animation_Step;

private:
	static AColor Red_Higlight, Blue_Highlight;
	HRGN Region;
};
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Multihit: public AActive_Brick
{
public:
	~AActive_Brick_Multihit();
	AActive_Brick_Multihit(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_rect);
	virtual bool Is_Finished();
	static void Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);
	/*static const int Max_Animation_Step = 15;
	int Animation_Step;*/
private:
	static void Draw_Stage(HDC hdc, RECT &brick_rect, int x, int width);

	int Rotation_Step;
	static const int Max_Rotation_Step = 32;
};
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
enum ETeleport_State
{
	ETS_Starting,
	ETS_Finishing,
	ETS_Done
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Teleport: public AActive_Brick
{
public:
	~AActive_Brick_Teleport();
	AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport *destination_teleport);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_rect);
	virtual bool Is_Finished();
	void Set_Ball(ABall *ball);

	static void Draw_In_Level(HDC hdc, RECT &brick_rect, int step = 0);

	EDirection_Type Release_Direction;
private:
	ETeleport_State Teleport_State;
	ABall *Ball;
	AActive_Brick_Teleport *Destination_Teleport;
	static const int Max_Animation_Step = 12;
	int Animation_Step;
};
//------------------------------------------------------------------------------------------------------------
class AAdvertisement: public AGraphics_Object
{
public:
	~AAdvertisement();
	AAdvertisement(int level_x, int level_y, int width, int height);
	void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual bool Is_Finished();
	void Show_Under_Brick(int level_x, int level_y);
	bool Has_Brick_At_Position(int level_x, int level_y);

private:
	int Level_X, Level_Y;
	int Width, Height;
	RECT Ad_Rect;
	char *Brick_Mask;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Ad: public AActive_Brick
{
public:
	~AActive_Brick_Ad();
	AActive_Brick_Ad(EBrick_Type brick_type, int level_x, int level_y, AAdvertisement *Advertisement);
	
	virtual void Draw(HDC hdc, RECT &paint_rect);
	virtual void Act();
	virtual bool Is_Finished();
	static void Draw_In_Level(HDC hdc, RECT &brick_rect);

private:
	static AColor Red_Higlight, Blue_Highlight;
	static const int Circle_Size = 7;
	AAdvertisement *Advertisement;
};
//------------------------------------------------------------------------------------------------------------