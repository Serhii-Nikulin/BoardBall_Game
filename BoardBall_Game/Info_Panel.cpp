#include "Info_Panel.h"


//AsInfo_Panel
//------------------------------------------------------------------------------------------------------------
AsInfo_Panel::~AsInfo_Panel()
{
	if (Logo_Font != 0)
		DeleteObject(Logo_Font);

	if (Name_Font != 0)
		DeleteObject(Name_Font);

	if (Score_Font != 0)
		DeleteObject(Score_Font);

	delete White_Color_1_px;
	delete Black_Color_1_px;

	delete Blue_Black;
	delete Red_Black;
}
//------------------------------------------------------------------------------------------------------------
AsInfo_Panel::AsInfo_Panel()
	: Logo_Font(0), Name_Font(0), Score_Font(0), White_Color_1_px(0), Black_Color_1_px(0), Red_Black(0), Blue_Black(0), 
	Floor_Table(EBrick_Type::Blue, ELetter_Type::P, 216 * AsConfig::Global_Scale, 153 * AsConfig::Global_Scale), Lifes_Table(EBrick_Type::Blue, ELetter_Type::G, 255 * AsConfig::Global_Scale + 1, 153 * AsConfig::Global_Scale), Monster_Table(EBrick_Type::Blue, ELetter_Type::M, 295 * AsConfig::Global_Scale, 153 * AsConfig::Global_Scale)
{
	Floor_Table.Show_Full_Size();
	Lifes_Table.Show_Full_Size();
	Monster_Table.Show_Full_Size();
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Begin_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Finish_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Shift_Per_Step(double max_speed)
{
}
//------------------------------------------------------------------------------------------------------------
double AsInfo_Panel::Get_Speed()
{
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Act()
{
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw(HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT rect{};
	const int &scale = AsConfig::Global_Scale;

	const wchar_t *board_str = L"Board";
	const wchar_t *ball_str = L"Ball";
	const wchar_t *gamer_name_str = L"Computer";
	const wchar_t *score_str = L"SCORE:000000";

	int logo_x_pos = 211 * scale;
	int logo_y_pos = 11 * scale;
	const int shadow_offset = 3 * scale;

	//Logotype
	AsTools::Rect(hdc, 208, 3, 110, 100, AsConfig::Blue_Color);

	SelectObject(hdc, Logo_Font);

	//Shadow
	SetBkColor(hdc, AsConfig::Blue_Color.Get_RGB() );
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );
	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, logo_x_pos + shadow_offset, logo_y_pos + shadow_offset, board_str, (int)wcslen(board_str) );
	TextOut(hdc, logo_x_pos + 18 * scale + shadow_offset, logo_y_pos + 37 * scale + shadow_offset, ball_str, (int)wcslen(ball_str) );

	//Strings
	SetBkColor(hdc, AsConfig::Blue_Color.Get_RGB() );
	SetTextColor(hdc, AsConfig::Red_Color.Get_RGB() );
	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, logo_x_pos, logo_y_pos, board_str, (int)wcslen(board_str));
	TextOut(hdc, logo_x_pos + 18 * scale, logo_y_pos + 37 * scale, ball_str, (int)wcslen(ball_str));

	//Score table
	//Red_Black board
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, Score_Width, 2, *Red_Black);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos + Score_Height - 2, Score_Width, 2, *Red_Black);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, 2, Score_Height, *Red_Black);
	AsTools::Rect(hdc, Score_X_Pos + Score_Width - 2, Score_Y_Pos, 2, Score_Height, *Red_Black);

	//Blue_Black table
	AsTools::Rect(hdc, Score_X_Pos + 2, Score_Y_Pos + 2, Score_Width - 4, Score_Height - 3, *Blue_Black);

	//Light board
	White_Color_1_px->Select_Pen(hdc);

	MoveToEx(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale, 0);
	LineTo(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + 2) * scale);
	LineTo(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + 2) * scale);

	//Black board
	Black_Color_1_px->Select_Pen(hdc);

	LineTo(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + 2) * scale);
	LineTo(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale);
	LineTo(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale);

	//Gamer name
	rect.left = (Score_X_Pos + 5) * scale;
	rect.top = (Score_Y_Pos + 5) * scale;
	rect.right = rect.left + (Score_Width - 2 * 5) * scale;
	rect.bottom = rect.top + 16 * scale;
	AsTools::Rect(hdc, rect, *Red_Black);

	Draw_String(hdc, rect, gamer_name_str, Name_Font);

	//Score number
	rect.top += Score_Value_Offset * scale;
	rect.bottom += Score_Value_Offset * scale;
	AsTools::Rect(hdc, rect, *Red_Black);

	Draw_String(hdc, rect, score_str, Score_Font);

	//Indicators
	//Floor
	Floor_Table.Draw(hdc, paint_area);
	AsTools::Rect(hdc, Score_X_Pos + 10, Score_Y_Pos + 58, 12, 30, AsConfig::Teleport_Color);	

	//Lifes
	Lifes_Table.Draw(hdc, paint_area);
	AsTools::Rect(hdc, Score_X_Pos + 27, Score_Y_Pos + 58, 55, 30, AsConfig::Teleport_Color);	

	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++)
			Draw_Extra_Life(hdc, (Score_X_Pos + 30 + i * 17), (Score_Y_Pos + 62 + j * 6) );

	//Monsters
	Monster_Table.Draw(hdc, paint_area);
	AsTools::Rect(hdc, Score_X_Pos + 89, Score_Y_Pos + 58, 12, 30, AsConfig::Teleport_Color);	

}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Extra_Life(HDC hdc, int x_pos, int y_pos)
{
	const int &scale = AsConfig::Global_Scale;
	RECT rect{};

	//left ellipse
	rect.left = x_pos * scale;
	rect.top = y_pos * scale;
	rect.right = rect.left + 5 * scale;
	rect.bottom = rect.top + 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	//right ellipse
	rect.left += 10 * scale;
	rect.right += 10 * scale;;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	//center rect
	rect.left = (x_pos + 3) * scale;
	rect.top = (y_pos + 1) * scale;
	rect.right = rect.left + 9 * scale;
	rect.bottom = rect.top + 3 * scale;

	AsTools::Rect(hdc, rect, AsConfig::Blue_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_String(HDC hdc, const RECT &rect, const wchar_t *string, HFONT font)
{
	SIZE size;
	int string_x_offset;
	int string_y_offset;
	const int &scale = AsConfig::Global_Scale;

	SelectObject(hdc, font);

	GetTextExtentPoint32(hdc, string, (int)wcslen(string), &size);

	string_x_offset = rect.left + (rect.right - rect.left) / 2 - size.cx / 2;
	string_y_offset = rect.top + (rect.bottom - rect.top) / 2 - size.cy / 2 - scale;

	//Draw shadow
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );
	TextOut(hdc, string_x_offset + 2 * scale, string_y_offset + 2 * scale, string, (int)wcslen(string) );

	//Draw string
	SetTextColor(hdc, AsConfig::White_Color.Get_RGB() );
	TextOut(hdc, string_x_offset, string_y_offset, string, (int)wcslen(string) );
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Clear_Prev_Animation(HDC hdc, RECT &paint_area)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Is_Finished()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Init()
{
	LOGFONT log_font{};

	log_font.lfHeight = -96L;
	log_font.lfWeight = 900L;
	log_font.lfCharSet = 204;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 34;
	wcscpy_s(log_font.lfFaceName, L"Arial Black");

	Logo_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -48;
	log_font.lfWeight = 700L;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 49;
	wcscpy_s(log_font.lfFaceName, L"Consolas");

	Name_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -44;
	Score_Font = CreateFontIndirect(&log_font);

	White_Color_1_px = new AColor(AsConfig::White_Color, AsConfig::Global_Scale);
	Black_Color_1_px = new AColor(AsConfig::BG_Color, AsConfig::Global_Scale);

	Blue_Black = new AColor(40, 45, 128);
	Red_Black = new AColor(128, 21, 19);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Choose_Font()
{
	CHOOSEFONT cf{};
	LOGFONT lf{};

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS;
	cf.nFontType = SCREEN_FONTTYPE;

	ChooseFont(&cf);
	Logo_Font = CreateFontIndirect(cf.lpLogFont);
}
//------------------------------------------------------------------------------------------------------------