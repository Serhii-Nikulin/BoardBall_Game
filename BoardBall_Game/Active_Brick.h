#pragma once
#include "Config.h"

enum EBrick_Type {
	EBT_None, EBT_Red, EBT_Blue,
	EBT_Unbreakable
};
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Draw(HDC HDC, RECT &paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
protected:
	virtual ~AActive_Brick();
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);
	EBrick_Type Brick_Type;
	RECT Brick_Rect;

};
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
