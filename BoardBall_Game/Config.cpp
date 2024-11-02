#include "Config.h"

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Color(237, 38, 36);
const AColor AsConfig::Blue_Color(63, 72, 204);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);

const AColor AsConfig::Red_Higlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);

const AColor AsConfig::Teleport_Color(AsConfig::Blue_Color, AsConfig::Global_Scale, AsConfig::BG_Color);

const AColor AsConfig::Advert_Blue_Table_Color(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Advert_Red_Table_Color(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);

const AColor AsConfig::Monster_Dark_Red_Color(191, 31, 31);
const AColor AsConfig::Monster_Cornea_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3, AsConfig::White_Color);
const AColor AsConfig::Monster_Iris_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3, AsConfig::Blue_Color);
const AColor AsConfig::BG_Outline_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3);

const AColor AsConfig::Explosion_Red_Color(AsConfig::White_Color, 0, AsConfig::Red_Color);
const AColor AsConfig::Explosion_Blue_Color(AsConfig::White_Color, 0, AsConfig::Blue_Color);

const double AsConfig::Acceleration = 1.001;
const double AsConfig::Ball_Normal_Speed = 3.0 * 2;

const double AsConfig::Ball_Radius = 2.0;
//------------------------------------------------------------------------------------------------------------
HWND AsConfig::Hwnd;

double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Has_Floor = true;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const unsigned char r, const unsigned char g, const unsigned char b, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor& color, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------




//AsTools
//------------------------------------------------------------------------------------------------------------
unsigned char AsTools::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_fade_step)
{
	return color - color * step / (max_fade_step - 1) + bg_color * step / (max_fade_step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color, int max_fade_step)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step, max_fade_step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step, max_fade_step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step, max_fade_step);

	result_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{
	return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;
	color.Select(hdc);

	Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
	int radius = corner_radius * AsConfig::Global_Scale;
	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AsTools::Reflect_On_Circle(double next_x_pos, double next_y_pos, double circle_x, double circle_y, double circle_radius, ABall_Object *ball)
{
	double dx, dy;
	double distance, two_radiuses;
	double alpha, beta, gamma;
	double related_ball_direction;
	const double pi_2 = 2.0 * M_PI;

	dx = next_x_pos - circle_x;
	dy = next_y_pos - circle_y;

	distance = sqrt(dx * dx + dy * dy);
	two_radiuses = circle_radius + AsConfig::Ball_Radius;

	if (distance + AsConfig::Moving_Step_Size < two_radiuses)
	{
		beta = atan2(-dy, dx);

		related_ball_direction = ball->Get_Direction();
		related_ball_direction -= beta;

		if (related_ball_direction > pi_2)
			related_ball_direction -= pi_2;

		if (related_ball_direction < 0.0)
			related_ball_direction += pi_2;

		if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
		{
			alpha = beta + M_PI - ball->Get_Direction();
			gamma = beta + alpha;

			ball->Set_Direction(gamma);

			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------



//AHit_Checker
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double next_pos, double eval_dist, double radius, double min_value, double max_value)
{
	if (eval_dist > radius)
		return false;

	//value * value + eval_pos * eval_pos = radius * radius

	double value = sqrt(radius * radius - eval_dist * eval_dist);

	if ((next_pos + value) > min_value and (next_pos - value) < max_value)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(double x_pos, double y_pos)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
AHit_Checker_List::AHit_Checker_List()
	: Counter_Hit_Checker(0), Hit_Checkers{}
{
}
//------------------------------------------------------------------------------------------------------------
void AHit_Checker_List::Add_Hit_Checker(AHit_Checker *hit_checker)
{
	if (Counter_Hit_Checker < Hit_Checkers_Count)
		Hit_Checkers[Counter_Hit_Checker++] = hit_checker;
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
	int i;

	for (i = 0; i < Counter_Hit_Checker; ++i)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall_Object *ball)
{
	int i;
	for (i = 0; i < Hit_Checkers_Count; ++i)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos, ball) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
