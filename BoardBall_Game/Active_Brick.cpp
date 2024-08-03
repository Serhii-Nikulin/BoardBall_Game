#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
//AGraphics_Object
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Level_Pos(int &brick_x, int &brick_y)
{
	brick_x = Level_X;
	brick_y = Level_Y;
}
//------------------------------------------------------------------------------------------------------------
//AActive_Brick
AActive_Brick::~AActive_Brick()
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
	:Brick_Type(brick_type), Level_X(level_x), Level_Y(level_y)
{
	Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width * level_x) * AsConfig::Global_Scale;
	Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height * level_y) * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
double AActive_Brick::Get_Brick_X_Pos(bool is_center)
{
	double x_pos = AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width;

	if (is_center)
		x_pos += AsConfig::Brick_Width / 2.0 + 1.0 / AsConfig::Global_Scale;;

	return x_pos;
}
//------------------------------------------------------------------------------------------------------------
double AActive_Brick::Get_Brick_Y_Pos(bool is_center)
{
	double y_pos = AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height;

	if (is_center)
		y_pos += AsConfig::Brick_Height / 2.0 + 1.0 / AsConfig::Global_Scale;;

	return y_pos;
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Red_Blue
//------------------------------------------------------------------------------------------------------------
AColor AActive_Brick_Red_Blue::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y) :
	AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
	if (!(brick_type == EBT_Blue or brick_type == EBT_Red))
		throw 13;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Act()
{
	if (++Fade_Step < Max_Fade_Step)
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	AColor* color = 0;

	switch (Brick_Type)
	{
	case EBT_Red:
		color = &Fading_Red_Brick_Colors[Fade_Step];
		break;

	case EBT_Blue:
		color = &Fading_Blue_Brick_Colors[Fade_Step];
		break;

	default:
		AsConfig::Throw();
	}

	//if (IntersectRect(&intersection_rect, &paint_area, &Brick_Rect))
	{
		if (color != 0)
			color->Select(hdc);

		AsConfig::Round_Rect(hdc, Brick_Rect);
	}

}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Red_Blue::Is_Finished()
{
	if (Fade_Step == Max_Fade_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Setup_Colors()
{
	int i;

	for (i = 0; i < Max_Fade_Step; i++)
	{
		Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
		Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{
	const AColor *color = 0;

	switch (brick_type)
	{
	case EBT_None:
		color = &AsConfig::BG_Color;
		break;

	case EBT_Blue:
		color = &AsConfig::Blue_Color;
		break;

	case EBT_Red:
		color = &AsConfig::Red_Color;
		break;

	case EBT_Unbreakable:
		color = &AsConfig::White_Color;
		break;

	default:
		AsConfig::Throw();
	}

	if (color != 0)
		color->Select(hdc);

	AsConfig::Round_Rect(hdc, brick_rect);
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - color * step / (Max_Fade_Step - 1) + bg_color * step / (Max_Fade_Step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

	result_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
	DeleteObject(Region);
}
// //------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(EBrick_Type brick_type, int level_x, int level_y) :
	AActive_Brick(brick_type, level_x, level_y), Animation_Step(0)
{
	Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_rect)
{
	const int scale = AsConfig::Global_Scale;
	int offset;

	Draw_In_Level(hdc, Brick_Rect);

	AsConfig::Red_Color.Select(hdc);

	SelectClipRgn(hdc, Region);

	AsConfig::Red_Higlight.Select_Pen(hdc);

	offset = (2 * Animation_Step - AsConfig::Brick_Width) * scale;

	MoveToEx(hdc, Brick_Rect.left + 2 * scale + offset, Brick_Rect.top + 8 * scale, 0);
	LineTo(hdc, Brick_Rect.left + 11 * scale + offset, Brick_Rect.top - 1 * scale);

	AsConfig::Blue_Highlight.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left + 0 * scale + offset, Brick_Rect.top + 8 * scale, 0);
	LineTo(hdc, Brick_Rect.left + 10 * scale + offset, Brick_Rect.top - 2 * scale);

	SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
	if (Animation_Step < Max_Animation_Step)
	{
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
		Animation_Step += 1;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
	if (Animation_Step >= Max_Animation_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT &brick_rect)
{
	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Multihit
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Multihit::AActive_Brick_Multihit(EBrick_Type brick_type, int level_x, int level_y)
	:AActive_Brick(brick_type, level_x, level_y), Rotation_Step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Act()
{ 
	if (Rotation_Step <= Max_Rotation_Step)
	{
		++Rotation_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw(HDC hdc, RECT &paint_rect)
{
	const int scale = AsConfig::Global_Scale;
	double x_delta;
	double angle;
	XFORM xform, prev_xform;
	RECT zero_rect;

	AsConfig::BG_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, Brick_Rect);

	angle = 4.0 * M_PI * (double)Rotation_Step / Max_Rotation_Step;
	x_delta = cos(angle);
	xform.eM11 = (FLOAT)x_delta; xform.eM12 = (FLOAT)0.0;
	xform.eM21 = (FLOAT)0.0; xform.eM22 = (FLOAT)1.0;

	xform.eDx = (FLOAT)(Brick_Rect.left + (1 - x_delta) * (AsConfig::Brick_Width * scale) / 2.0);
	xform.eDy = (FLOAT)Brick_Rect.top;
	GetWorldTransform(hdc, &prev_xform);
	SetWorldTransform(hdc, &xform);

	AsConfig::Letter_Color.Select_Pen(hdc);
	
	MoveToEx(hdc, 0 + 1 * scale, 0 + 3 * scale, 0);
	LineTo(hdc, 0 + 3 * scale, 0 + 1 * scale);
	LineTo(hdc, 0 + 3 * scale, 0 + 6 * scale + 1);
	
	RoundRect(hdc, 0 + 5 * scale, 0 + 1 * scale, 0 + 8 * scale, 0 + 6 * scale + 1, 2 * scale, 2 * scale);
	RoundRect(hdc, 0 + 10 * scale, 0 + 1 * scale, 0 + 13 * scale, 0 + 6 * scale + 1, 2 * scale, 2 * scale);

	SetWorldTransform(hdc, &prev_xform);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Multihit::Is_Finished()
{
	if (Rotation_Step >= Max_Rotation_Step)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{
	const int scale = AsConfig::Global_Scale;
	

	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	AsConfig::Red_Color.Select(hdc);
	Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

	switch (brick_type)
	{
	case EBT_Multihit_1:
		Draw_Stage(hdc, brick_rect, 2, 10);
		break;
		
	case EBT_Multihit_2:
		Draw_Stage(hdc, brick_rect, 2, 4);
		Draw_Stage(hdc, brick_rect, 8, 4);
		break;

	case EBT_Multihit_3:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 6, 2);
		Draw_Stage(hdc, brick_rect, 10, 2);
		break;

	case EBT_Multihit_4:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 5, 2);
		Draw_Stage(hdc, brick_rect, 8, 2);
		Draw_Stage(hdc, brick_rect, 11, 2);
		break;

	default:
		AsConfig::Throw();
	}

}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT &brick_rect, int x, int width)
{
	RECT stage_rect;
	const int scale = AsConfig::Global_Scale;

	stage_rect.left = brick_rect.left + x * scale;
	stage_rect.top = brick_rect.top + 2 * scale;
	stage_rect.right = stage_rect.left + width * scale - 1;
	stage_rect.bottom = stage_rect.top + 3 * scale - 1;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale, stage_rect.bottom + scale);

	AsConfig::Blue_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right, stage_rect.bottom);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Teleport
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::~AActive_Brick_Teleport()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Teleport::AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport *destination_teleport)
	:AActive_Brick(EBT_Teleport, level_x, level_y), Teleport_State(ETS_Starting), Animation_Step(0), Ball(ball), Destination_Teleport(destination_teleport)
{
	Set_Ball(ball);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Act()
{ 
	double ball_x = 0, ball_y = 0;
	double direction;

	if (Animation_Step <= Max_Animation_Step)
	{
		++Animation_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
	else
	{
		switch (Teleport_State)
		{
		case ETS_Starting:
			Animation_Step = 0;
			Teleport_State = ETS_Finishing;

			if (Destination_Teleport != 0)
			{
				Destination_Teleport->Ball = Ball;
				Destination_Teleport->Set_Ball(Ball);
				Ball = 0;
			}
			break;

		case ETS_Finishing:
			Teleport_State = ETS_Done;

			if (Ball != 0)
			{
				switch (Release_Direction)
				{
				case EDT_Left:
					ball_x = Get_Brick_X_Pos(false) - ABall::Radius;
					ball_y = Get_Brick_Y_Pos(true);
					break;

				case EDT_Up:
					ball_x = Get_Brick_X_Pos(true);
					ball_y = Get_Brick_Y_Pos(false) - ABall::Radius;
					break;

				case EDT_Right:
					ball_x = Get_Brick_X_Pos(false) + AsConfig::Brick_Width + ABall::Radius;
					ball_y = Get_Brick_Y_Pos(true);
					break;

				case EDT_Down:
					ball_x = Get_Brick_X_Pos(true);
					ball_y = Get_Brick_Y_Pos(false) + AsConfig::Brick_Height + ABall::Radius;
					break;

				default:
					AsConfig::Throw();
				}

				direction = Ball->Get_Direction();
				Ball->Set_State(EBS_Normal, ball_x, ball_y, direction);
				Ball = 0;
			
			}
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw(HDC hdc, RECT &paint_rect)
{
	int step;

	switch (Teleport_State)
	{
	case ETS_Starting:
		step = Animation_Step;
		break;

	case ETS_Finishing:
		step = Max_Animation_Step - Animation_Step;
		break;

	default:
		step = 0;
		return;
	}

	Draw_In_Level(hdc, Brick_Rect, step);

	if (Ball != 0)
		Ball->Draw_Teleporting(hdc, step);
	
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Teleport::Is_Finished()
{
	if (Teleport_State == ETS_Done)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Set_Ball(ABall *ball)
{
	double ball_x, ball_y;

	ball_x = Get_Brick_X_Pos(true);
	ball_y = Get_Brick_Y_Pos(true);

	if (ball != 0)
		Ball->Set_State(EBS_Teleporting, ball_x, ball_y);

	Ball = ball;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Teleport::Draw_In_Level(HDC hdc, RECT &brick_rect, int step)
{
	const int scale = AsConfig::Global_Scale;

	int top_y = brick_rect.top + 0 * scale + step / 2 + 1;
	int low_y = brick_rect.top + 6 * scale - step / 2 + 1;

	AsConfig::Red_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	AsConfig::Teleport_Color.Select(hdc);
	Ellipse(hdc, brick_rect.left + 3 * scale + 1, top_y, brick_rect.left + 11 * scale + 1, low_y);
}
//------------------------------------------------------------------------------------------------------------




//AAdvertisement
//------------------------------------------------------------------------------------------------------------
AAdvertisement::~AAdvertisement()
{
	int i, j;
	HRGN region;

	for (i = 0; i < Height; i ++)
		for (j = 0; j < Width; j++)
		{
			region = Brick_Regions[i * Width + j];

			if (region != 0)
				DeleteObject(region);
		}

	delete[] Brick_Regions;
	Brick_Regions = 0;
}
//------------------------------------------------------------------------------------------------------------
AAdvertisement::AAdvertisement(int level_x, int level_y, int width, int height)
	:Level_X(level_x), Level_Y(level_y), Width(width), Height(height), Brick_Regions(0)
{
	Empty_Region = CreateRectRgn(0, 0, 0, 0);

	Brick_Regions = new HRGN[Width * Height];
	memset(Brick_Regions, 0, Width * Height * sizeof(HRGN) );

	Ad_Rect.left = (AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	Ad_Rect.top = (AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	Ad_Rect.right = Ad_Rect.left + ((Width - 1) * AsConfig::Cell_Width + AsConfig::Brick_Width) * AsConfig::Global_Scale;
	Ad_Rect.bottom = Ad_Rect.top + ((Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height) * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AAdvertisement::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{	
}
//------------------------------------------------------------------------------------------------------------
void AAdvertisement::Act()
{
	RECT rect;
	int i, j;

	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			if (Brick_Regions[i * Width + j] != 0)
			{
				rect.left = Ad_Rect.left + j * AsConfig::Cell_Width * AsConfig::Global_Scale;
				rect.top = Ad_Rect.top + i * AsConfig::Cell_Height * AsConfig::Global_Scale;
				rect.right = rect.left + AsConfig::Cell_Width * AsConfig::Global_Scale;
				rect.bottom = rect.top + AsConfig::Cell_Width * AsConfig::Global_Scale;

				InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
			}
}
//------------------------------------------------------------------------------------------------------------
void AAdvertisement::Draw(HDC hdc, RECT &paint_area)
{
	HRGN Region;
	RECT intersection_rect;

	int i, j;
	int x, y;
	const int &scale = AsConfig::Global_Scale;

	if (!IntersectRect(&intersection_rect, &paint_area, &Ad_Rect))
		return;
	
	SelectClipRgn(hdc, Empty_Region);

	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
		{
			Region = Brick_Regions[i * Width + j];
			if (Region != 0)
				ExtSelectClipRgn(hdc, Region, RGN_OR);
		}

	AsConfig::BG_Color.Select(hdc);
	AsConfig::Blue_Color.Select_Pen(hdc);
	AsConfig::Round_Rect(hdc, Ad_Rect);
	POINT table_points[4] = 
	{
		{Ad_Rect.left + 1, Ad_Rect.top + 14 * scale}, 
		{Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 8 * scale}, 
		{Ad_Rect.left + 30 * scale, Ad_Rect.top + 14 * scale}, 
		{Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale}
	};

	//white surface
	AsConfig::White_Color.Select(hdc);
	Polygon(hdc, table_points, 4);
	
	//blue table part
	AsConfig::Advert_Blue_Table_Color.Select(hdc);
	MoveToEx(hdc, Ad_Rect.left + 1 * scale - 1, Ad_Rect.top + 14 * scale, 0);
	LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 8 * scale);
	LineTo(hdc, Ad_Rect.left + 30 * scale, Ad_Rect.top + 14 * scale);
	LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale);
	LineTo(hdc, Ad_Rect.left + 1 * scale - 1, Ad_Rect.top + 14 * scale);

	//red part
	AsConfig::Advert_Red_Table_Color.Select(hdc);
	MoveToEx(hdc, Ad_Rect.left + 1 * scale, Ad_Rect.top + 16 * scale - 1, 0);
	LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 22 * scale - 1);
	LineTo(hdc, Ad_Rect.left + 30 * scale - 1, Ad_Rect.top + 16 * scale - 1);

	//ball's shadow
	x = Ad_Rect.left + 9 * scale + 1;
	y = Ad_Rect.top + 3 * scale;
	AsConfig::Blue_Color.Select(hdc);
	

	//ball
	AsConfig::Red_Color.Select(hdc);
	Ellipse(hdc, x, y, x + Ball_Size, y + Ball_Size);//ball

	//ball's highlight
	AsConfig::Letter_Color.Select(hdc);
	Arc(hdc,
		x + 2 * AsConfig::Global_Scale - 1, y + 2 * AsConfig::Global_Scale - 1,
		x + Ball_Size - 2 * AsConfig::Global_Scale + 1, y + Ball_Size - 2 * AsConfig::Global_Scale + 1,
		x + (Ball_Size / 2) - 2 * AsConfig::Global_Scale, y,
		x, y + (Ball_Size / 2) - 2 * AsConfig::Global_Scale);

	SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AAdvertisement::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AAdvertisement::Show_Under_Brick(int level_x, int level_y)
{
	RECT region_rect;

	int i, j;
	const int &scale = AsConfig::Global_Scale;
	int x = level_x - Level_X;
	int y = level_y - Level_Y;

	if (x < 0 or x >= Width)
		AsConfig::Throw();

	if (y < 0 or y >= Height)
		AsConfig::Throw();

	region_rect.left = Ad_Rect.left + x * AsConfig::Cell_Width * scale;
	region_rect.top = Ad_Rect.top + y * AsConfig::Cell_Height * scale;
	region_rect.right = region_rect.left + AsConfig::Cell_Width * scale;
	region_rect.bottom = region_rect.top + AsConfig::Cell_Height * scale;

	Brick_Regions[y * Width + x] = CreateRectRgnIndirect(&region_rect);
}
//------------------------------------------------------------------------------------------------------------
bool AAdvertisement::Has_Brick_At_Position(int level_x, int level_y)
{
	if (level_x >= Level_X and level_x < Level_X + Width)
		if (level_y >= Level_Y and level_y < Level_Y + Height)
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::~AActive_Brick_Ad()
{
}
// //------------------------------------------------------------------------------------------------------------
AActive_Brick_Ad::AActive_Brick_Ad(EBrick_Type brick_type, int level_x, int level_y, AAdvertisement *advertisement) :
	AActive_Brick(brick_type, level_x, level_y), Advertisement(advertisement)
{
	if (Advertisement != 0)
		Advertisement->Show_Under_Brick(Level_X, Level_Y);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw(HDC hdc, RECT& paint_rect)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Act()
{
	//InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Ad::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Ad::Draw_In_Level(HDC hdc, RECT &brick_rect)
{
	int i;
	int x = brick_rect.left;
	int y = brick_rect.top;
	const int scale = AsConfig::Global_Scale;
	int offset = 8 * scale;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, brick_rect.left, brick_rect.top, brick_rect.right + scale, brick_rect.bottom + scale);

	for (i = 0; i < 2; i++)
	{
		AsConfig::Red_Color.Select(hdc);
		Ellipse(hdc, x, y, x + Circle_Size * scale - 1, y + Circle_Size * scale - 1);

		AsConfig::White_Color.Select(hdc);
		Arc(hdc,
			x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
			x + (Circle_Size - 1) * scale, y + (Circle_Size - 1) * scale,
			x + (Circle_Size / 2) * scale, y,
			x, y + (Circle_Size / 2) * scale);

		x += offset;
	}
	
}
//------------------------------------------------------------------------------------------------------------
