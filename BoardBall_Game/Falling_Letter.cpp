#include "Falling_Letter.h"

int AFalling_Letter::All_Letters_Popularity;
const int AFalling_Letter::Letters_Popularity[(char)ELetter_Type::Max] = {7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 1};
//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
	:Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Rotation_Step(2), Falling_Letter_State(EFalling_Letter_State::Normal)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = X + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Letter_Cell.bottom = Y + AsConfig::Brick_Height * AsConfig::Global_Scale;
	Prev_Letter_Cell = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{
	bool switch_color;
	double offset;
	double rotation_angle;
	double y_ratio;
	int back_part_offset;
	int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
	const AColor *front_color, *back_color;
	XFORM xform, prev_xform;
	RECT letter_rect;
	letter_rect.left = (AsConfig::Brick_Width - (AsConfig::Brick_Height - 2)) * AsConfig::Global_Scale / 2;
	letter_rect.top = -(AsConfig::Brick_Height - 2) * AsConfig::Global_Scale / 2;
	letter_rect.right = (AsConfig::Brick_Width + (AsConfig::Brick_Height - 2)) * AsConfig::Global_Scale / 2;
	letter_rect.bottom = +(AsConfig::Brick_Height - 2) * AsConfig::Global_Scale / 2;
	int middle_letter_x = letter_rect.left + (letter_rect.right - letter_rect.left) / 2;
	int middle_letter_y = letter_rect.top + (letter_rect.bottom - letter_rect.top) / 2;

	if (! (Brick_Type == EBrick_Type::Blue || Brick_Type == EBrick_Type::Red))
		return;

	Rotation_Step %= Max_Rotation_Step;

	if (Rotation_Step < 8)
		rotation_angle = 2.0 * M_PI * Rotation_Step / (double)Max_Rotation_Step;
	else
		rotation_angle = 2.0 * M_PI * (8 - Rotation_Step) / (double)Max_Rotation_Step;

	if (Rotation_Step > 4 and Rotation_Step <= 12)
	{
		if (Brick_Type == EBrick_Type::Blue)
			switch_color = true;
		else
			switch_color = false;
	}
	else
	{
		if (Brick_Type == EBrick_Type::Blue)
			switch_color = false;
		else
			switch_color = true;
	}

	Set_Brick_Letter_Colors(switch_color, &front_color, &back_color);

	if (Rotation_Step == 4 or Rotation_Step == 12)
	{
		back_color->Select(hdc);
		Rectangle(hdc, X, Y + brick_half_height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Y + brick_half_height);

		//Front side
		front_color->Select(hdc);
		Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Y + brick_half_height + AsConfig::Global_Scale - 1);
	}
	else
	{
		y_ratio = cos(rotation_angle);

		GetWorldTransform(hdc, &prev_xform);

		xform.eM11 = (FLOAT)1; xform.eM12 = (FLOAT)0;
		xform.eM21 = (FLOAT)0; xform.eM22 = (FLOAT)fabs(cos(rotation_angle) );
		xform.eDx = (FLOAT)X;
		xform.eDy = (FLOAT)Y + brick_half_height;

		SetWorldTransform(hdc, &xform);

		offset = 3.0 * (1.0f - fabs(cos(rotation_angle))) * AsConfig::Global_Scale;
		back_part_offset = (int)round(offset);

		if (y_ratio < 0.0)
			back_part_offset = -back_part_offset;

		back_color->Select(hdc);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, +brick_half_height - back_part_offset);

		//Front side
		front_color->Select(hdc);
		Rectangle(hdc, 0, -(int)brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, +(int)brick_half_height);

		if (Rotation_Step > 4 and Rotation_Step < 12)
		{
			AsConfig::Letter_Color.Select_Pen(hdc);

			switch (Letter_Type)
			{
			case ELetter_Type::O:
				Ellipse(hdc, letter_rect.left, letter_rect.top, letter_rect.right, letter_rect.bottom);
				break;

			case ELetter_Type::I:
				//left line
				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left, letter_rect.bottom);

				//right line
				MoveToEx(hdc, letter_rect.right, letter_rect.top, 0);	
				LineTo(hdc, letter_rect.right, letter_rect.bottom);

				//diagonal
				MoveToEx(hdc, letter_rect.left, letter_rect.bottom, 0);
				LineTo(hdc, letter_rect.right, letter_rect.top);
				
				break;

			case ELetter_Type::G:
				//middle horizontal line
				MoveToEx(hdc, letter_rect.left + 2, middle_letter_y, 0);
				LineTo(hdc, letter_rect.right - 2, middle_letter_y);

				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left + 2, middle_letter_y);

				MoveToEx(hdc, letter_rect.right, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right - 2, middle_letter_y);

				MoveToEx(hdc, letter_rect.left, letter_rect.bottom, 0);
				LineTo(hdc, letter_rect.left + 2, middle_letter_y);

				MoveToEx(hdc, letter_rect.right, letter_rect.bottom, 0);
				LineTo(hdc, letter_rect.right - 2, middle_letter_y);

				//middle vertical line
				MoveToEx(hdc, middle_letter_x + 1, letter_rect.top, 0);
				LineTo(hdc,  middle_letter_x, letter_rect.bottom);

				break;

			case ELetter_Type::M:
				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, middle_letter_x, letter_rect.bottom);

				MoveToEx(hdc, middle_letter_x, letter_rect.bottom, 0);
				LineTo(hdc, letter_rect.right, letter_rect.top);

				MoveToEx(hdc, letter_rect.right, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right, letter_rect.bottom);

				break;

			case ELetter_Type::K:
				MoveToEx(hdc, middle_letter_x - 5, letter_rect.top, 0);
				LineTo(hdc, middle_letter_x - 5, letter_rect.bottom);

				MoveToEx(hdc, middle_letter_x - 5, middle_letter_y, 0);
				LineTo(hdc, letter_rect.right - 5, letter_rect.top);

				MoveToEx(hdc, middle_letter_x - 5, middle_letter_y, 0);
				LineTo(hdc, letter_rect.right - 5, letter_rect.bottom);
				break;

			case ELetter_Type::W:
				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.right, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right, letter_rect.bottom);

				MoveToEx(hdc, middle_letter_x, letter_rect.top, 0);
				LineTo(hdc, middle_letter_x, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.left, letter_rect.bottom, 0);
				LineTo(hdc, letter_rect.right, letter_rect.bottom);
				break;

			case ELetter_Type::P:
				MoveToEx(hdc, letter_rect.left + 2, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left + 2, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.right - 2, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right - 2, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.left + 1, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right - 2, letter_rect.top);
				break;

			case ELetter_Type::L:
				MoveToEx(hdc, middle_letter_x, letter_rect.top, 0);
				LineTo(hdc, letter_rect.left + 2, letter_rect.bottom);

				MoveToEx(hdc, middle_letter_x, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right - 2, letter_rect.bottom);
				break;

			case ELetter_Type::T:
				MoveToEx(hdc, letter_rect.left, letter_rect.top, 0);
				LineTo(hdc, letter_rect.right, letter_rect.top);

				MoveToEx(hdc, middle_letter_x + 1, letter_rect.top, 0);
				LineTo(hdc, middle_letter_x + 1, letter_rect.bottom);
				break;

			case ELetter_Type::Plus:
				MoveToEx(hdc, middle_letter_x + 1, letter_rect.top, 0);
				LineTo(hdc, middle_letter_x + 1, letter_rect.bottom);

				MoveToEx(hdc, letter_rect.left, middle_letter_y, 0);
				LineTo(hdc, letter_rect.right, middle_letter_y);
				break;

			case ELetter_Type::C:
				Arc(hdc, letter_rect.left, letter_rect.top , letter_rect.right, letter_rect.bottom, 
					letter_rect.right - 1, letter_rect.top + 1, letter_rect.right, letter_rect.bottom - 1);
				break;

			default:
				AsConfig::Throw();
			}
			
		}
		SetWorldTransform(hdc, &prev_xform);
	}
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, const AColor **front_color, const AColor **back_color)
{
	if (is_switch_color)
	{
		*front_color  = &AsConfig::Red_Color;
		*back_color = &AsConfig::Blue_Color;
	}
	else
	{
		*front_color = &AsConfig::Blue_Color;
		*back_color = &AsConfig::Red_Color;
	}
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
	if (Falling_Letter_State != EFalling_Letter_State::Normal)
		return;

	if (Letter_Cell.top > AsConfig::Max_Y_Pos * AsConfig::Global_Scale)
	{
		Finalize();
		return;
	}

	Prev_Letter_Cell = Letter_Cell;

	Y += AsConfig::Global_Scale;
	Letter_Cell.top += AsConfig::Global_Scale;
	Letter_Cell.bottom += AsConfig::Global_Scale;

	Rotation_Step += 1;

	AsTools::Invalidate_Rect(Prev_Letter_Cell);
	AsTools::Invalidate_Rect(Letter_Cell);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
		return;

	AsTools::Rect(hdc, Prev_Letter_Cell, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (Falling_Letter_State == EFalling_Letter_State::Finalizing)
	{
		Falling_Letter_State = EFalling_Letter_State::Finished;
		return;
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
		Draw_Brick_Letter(hdc);
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	if (Falling_Letter_State == EFalling_Letter_State::Finished)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Get_Letter_Cell(RECT &rect)
{
	rect = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Finalize()
{
	Falling_Letter_State = EFalling_Letter_State::Finalizing;
	AsTools::Invalidate_Rect(Prev_Letter_Cell);
	AsTools::Invalidate_Rect(Letter_Cell);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
	int i;
	int x_step = AsConfig::Cell_Width * AsConfig::Global_Scale;

	Rotation_Step = 0;

	for (i = 0; i < Max_Rotation_Step; i++)
	{
		Draw_Brick_Letter(hdc);

		++Rotation_Step;
		X += x_step;
		Letter_Cell.left += x_step;
		Letter_Cell.right += x_step;		
	}
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Show_Full_Size()
{
	Rotation_Step = 8;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Init()
{
	int i;
	All_Letters_Popularity = 0;

	for (i = 0; (ELetter_Type)i < ELetter_Type::Max; i++)
		All_Letters_Popularity += Letters_Popularity[i];
}
//------------------------------------------------------------------------------------------------------------
ELetter_Type AFalling_Letter::Get_Random_Letter_Type()
{
	int i;
	int letter_popularity = AsTools::Rand(All_Letters_Popularity);

	for (i = 0; (ELetter_Type)i < ELetter_Type::Max; i++)
	{
		if (letter_popularity < Letters_Popularity[i])
			return (ELetter_Type)i;

		letter_popularity -= Letters_Popularity[i];
	}

	return ELetter_Type::O;
}
//------------------------------------------------------------------------------------------------------------