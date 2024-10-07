#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class EGate_State: unsigned char
{
	Closed,
	Short_Open,
	Long_Open
};
//------------------------------------------------------------------------------------------------------------
enum class EGate_Transformation: unsigned char
{ 
	Unknown, 
	Init, 
	Active, 
	Finalize
};
//------------------------------------------------------------------------------------------------------------
class AGate: public AGraphics_Object
{
public:
	AGate(int x_pos, int y_pos);
	virtual void Act();
	virtual void Draw(HDC HDC, RECT &paint_area);
	virtual void Clear_Prev_Animation(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Open_Gate(bool short_open);
	bool Is_Opened() const;
	void Get_Y_Limits(int &gate_top_y, int &gate_low_y) const;

private:
	void Draw_Cup(HDC hdc, bool is_top);
	void Draw_Edges(HDC hdc);
	void Draw_Short_Open_Edges(HDC hdc);
	void Draw_Long_Open_Edges(HDC hdc);
	void Draw_One_Edge(HDC hdc, int x, int y, bool is_longer_edge);
	void Draw_Red_Edge(HDC hdc, int x, int y, bool is_longer_edge, bool has_highlight);
	void Draw_Blazing(HDC hdc);

	bool Act_For_Open(bool short_opening, bool &correct_pos);
	void Redraw_Gate();

	int X_Pos;
	double Y_Pos, Origin_Y_Pos;
	const int Width = 6;
	const int Height = 19;

	double Gap_Height;

	static const int Edges_Count_Per_Cup = 6;

	static const double Max_Gap_Short_Height;
	static const double Max_Gap_Long_Height;

	static const double Gap_Height_Short_Step;
	static const double Gap_Height_Long_Step;

	int Gate_Close_Tick;
	static const int Short_Opening_Timeout = AsConfig::FPS;
	static const int Long_Opening_Timeout = AsConfig::FPS * 3;

	EGate_State Gate_State;
	EGate_Transformation Gate_Transformation;
	RECT Rect;
};
//------------------------------------------------------------------------------------------------------------