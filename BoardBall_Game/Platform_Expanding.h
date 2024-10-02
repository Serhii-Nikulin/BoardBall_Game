#include "Platform_State.h"

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