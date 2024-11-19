#include "Engine.h" 

//AsInfo_Panel
AsInfo_Panel::~AsInfo_Panel()
{
	DeleteObject(Logo_Font);

	delete White_Color_1_px;
	delete Black_Color_1_px;

	delete Blue_Black;
	delete Red_Black;
}
//------------------------------------------------------------------------------------------------------------
AsInfo_Panel::AsInfo_Panel()
: Logo_Font(0), Name_Font(0), White_Color_1_px(0), Black_Color_1_px(0), Red_Black(0), Blue_Black(0)
{
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
	const int &scale = AsConfig::Global_Scale;

	const wchar_t *board_str = L"Board";
	const wchar_t *ball_str = L"Ball";
	const wchar_t *name_str = L"Computer";

	int logo_x_pos = 211 * scale;
	int logo_y_pos = 11 * scale;
	const int shadow_offset = 3 * scale;
	int score_x_pos = 208;
	int score_y_pos = 106;
	int score_width = 110;
	int score_height = 92;

	//Logotype
	AsTools::Rect(hdc, 208, 3, 110, 100, AsConfig::Blue_Color);

	SelectObject(hdc, Logo_Font);

	//Shadow
	SetBkColor(hdc, AsConfig::Blue_Color.Get_RGB() );
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );
	SetBkMode(hdc, TRANSPARENT);
	
	TextOut(hdc, logo_x_pos + shadow_offset, logo_y_pos + shadow_offset, board_str, wcslen(board_str) );
	TextOut(hdc, logo_x_pos + 18 * scale + shadow_offset, logo_y_pos + 37 * scale + shadow_offset, ball_str, wcslen(ball_str) );

	//Strings
	SetBkColor(hdc, AsConfig::Blue_Color.Get_RGB() );
	SetTextColor(hdc, AsConfig::Red_Color.Get_RGB() );
	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, logo_x_pos, logo_y_pos, board_str, wcslen(board_str));
	TextOut(hdc, logo_x_pos + 18 * scale, logo_y_pos + 37 * scale, ball_str, wcslen(ball_str));

	//Score table
		//Red_Black board
	AsTools::Rect(hdc, score_x_pos, score_y_pos, score_width, 2, *Red_Black);
	AsTools::Rect(hdc, score_x_pos, score_y_pos + score_height - 2, score_width, 2, *Red_Black);
	AsTools::Rect(hdc, score_x_pos, score_y_pos, 2, score_height, *Red_Black);
	AsTools::Rect(hdc, score_x_pos + score_width - 2, score_y_pos, 2, score_height, *Red_Black);

		//Blue_Black table
	AsTools::Rect(hdc, score_x_pos + 2, score_y_pos + 2, score_width - 4, score_height - 3, *Blue_Black);

	//AsTools::Rect(hdc, score_x_pos, score_y_pos, score_width, score_height, AsConfig::Red_Color);

		//Light board
	White_Color_1_px->Select_Pen(hdc);

	MoveToEx(hdc, (score_x_pos + 2) * scale, (score_y_pos + score_height - 2) * scale, 0);
	LineTo(hdc, (score_x_pos + 2) * scale, (score_y_pos + 2) * scale);
	LineTo(hdc, (score_x_pos + score_width - 2) * scale, (score_y_pos + 2) * scale);

		//Black board
	Black_Color_1_px->Select_Pen(hdc);

	LineTo(hdc, (score_x_pos + score_width - 2) * scale, (score_y_pos + 2) * scale);
	LineTo(hdc, (score_x_pos + score_width - 2) * scale, (score_y_pos + score_height - 2) * scale);
	LineTo(hdc, (score_x_pos + 2) * scale, (score_y_pos + score_height - 2) * scale);

		//Gamer name
	AsTools::Rect(hdc, score_x_pos + 5, score_y_pos + 5, score_width - 2 * 5, 16, *Red_Black);

	SelectObject(hdc, Name_Font);
	SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB() );
	TextOut(hdc, (score_x_pos + 5) * scale, (score_y_pos + 2) * scale, name_str, wcslen(name_str));

		//Score number
	AsTools::Rect(hdc, score_x_pos + 5, score_y_pos + 27, score_width - 2 * 5, 16, *Red_Black);
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

	log_font.lfHeight = -96;
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



//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	:Game_State(EGame_State::Lost_Ball), Rest_Distance(0.0), Life_Count(AsConfig::Initial_Life_Count), Modules{}, Ball_Set(), Laser_Beam_Set(), Monster_Set(), Timer_ID(WM_USER + 1)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{
	SYSTEMTIME sys_time;
	FILETIME file_time;
	int index;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);
	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;

	ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	AsPlatform::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);
	
	AFalling_Letter::Init();
	Platform.Init(&Laser_Beam_Set, &Ball_Set);
	Level.Init();
	Monster_Set.Init(&Border);
	Level.Set_Current_Level(AsLevel::Level_01);
	Info_Panel.Init();
	/*Ball.Set_State(EBall_State::Normal);
	Platform.Set_State(EPS_Normal); */
	
	AActive_Brick_Red_Blue::Setup_Colors();
	AExplosive_Ball::Setup_Colors();

	Platform.Redraw();
	
	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, NULL);

	index = 0;
	memset(Modules, 0, sizeof(Modules) );
	Add_Next_Module(index, &Level); // 1
	Add_Next_Module(index, &Border); // 2 - floor effect
	Add_Next_Module(index, &Platform);
	Add_Next_Module(index, &Ball_Set);
	Add_Next_Module(index, &Laser_Beam_Set);
	Add_Next_Module(index, &Monster_Set);
	Add_Next_Module(index, &Info_Panel);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Clear_Prev_Animation(hdc, paint_area);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Move(true, key_down);
		break;

	case EKey_Type::Right:
		Platform.Move(false, key_down);
		break;

	case EKey_Type::Space:
		Platform.On_Space_Key(key_down);
		break;
	}

	return 0;
}

//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	AsConfig::Current_Timer_Tick += 1;

	switch (Game_State)
	{
	case EGame_State::Test_Ball:
		Ball_Set.Set_For_Test();
		Game_State = EGame_State::Play_Level;
		break;

	case EGame_State::Play_Level:
		Play_Level();
		break;

	case EGame_State::Lost_Ball:
		if (Platform.Has_State(EPlatform_Substate_Regular::Missing) )
		{
			Restart_Level();
		}
		break;

	case EGame_State::Restart_Level:
		if (Platform.Has_State(EPlatform_Substate_Regular::Ready) )
		{
			Game_State = EGame_State::Play_Level;
			Ball_Set.Set_On_Platform();
			Monster_Set.Activate(10);
		}
		break;
	}

	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
	Game_State = EGame_State::Restart_Level;
	Border.Open_Gate(AsConfig::Gates_Count - 1, true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	Shift_Movers();
	
	if (Ball_Set.All_Balls_Are_Lost() )
	{
		Game_State = EGame_State::Lost_Ball;
		Platform.Set_State(EPlatform_State::Meltdown);
		Level.Stop();
		Monster_Set.Destroy_All();
		Laser_Beam_Set.Disable_All();
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished() )
		Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int i;
	int index = 0;
	AFalling_Letter *falling_letter;

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Act();

	//if (! Platform.Has_State(EPlatform_Substate_Regular::Ready) )
	//	Ball_Set.Act();

	//Platform.Act();
	//Level.Act();
	//Border.Act();

	while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
	{
		if (Platform.Hit_By(falling_letter) )
			On_Falling_Letter(falling_letter);
	}

	if (Game_State == EGame_State::Restart_Level)
		if (Border.Is_Gate_Opened(AsConfig::Gates_Count - 1) )
			Platform.Set_State(EPlatform_State::Rolling);	
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELetter_Type::O: 
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;			
	case ELetter_Type::M: 
		break;
	case ELetter_Type::I: 
		Ball_Set.Inverse_Direction();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	case ELetter_Type::C:
		Ball_Set.Reset_Balls_Speed();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	case ELetter_Type::K:
		Platform.Set_State(EPlatform_State::Adhesive);
		break;
	case ELetter_Type::W: 
		Platform.Set_State(EPlatform_State::Expanding);
		break;				  
	case ELetter_Type::G:
		if (Life_Count < AsConfig::Max_Life_Count)
			++Life_Count;
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	case ELetter_Type::T: 
		Ball_Set.Triple_Ball();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	case ELetter_Type::L:
		Platform.Set_State(EPlatform_State::Laser);
		break;
	case ELetter_Type::P: 
		AsConfig::Has_Floor = true;
		Border.Redraw_Floor();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	case ELetter_Type::Plus:
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;
	default:
		AsConfig::Throw();
	}

	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Shift_Movers()
{
	int i;
	double current_speed, max_speed = 0;
	
	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
	{
		if (Modules[i] != 0)
		{
			current_speed = fabs(Modules[i]->Get_Speed() );

			if (current_speed > max_speed)
				max_speed = current_speed;
		}
	}

	Rest_Distance += max_speed;

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Begin_Movement();

	while (Rest_Distance > 0.0)
	{
		for (i = 0; i < AsConfig::Max_Modules_Count; i++)
			if (Modules[i] != 0)
				Modules[i]->Shift_Per_Step(max_speed);

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}

	/*double x, y;
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		Ball_Set.Balls[i].Get_Center(x, y);

		if (x >= Platform.X_Pos and x < Platform.X_Pos + Platform.Width)
			if (y >= AsConfig::Platform_Y_Pos and y < AsConfig::Platform_Y_Pos + 7)
 				int yy = 0; 
	}*/

 	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Add_Next_Module(int &index, AGame_Object *game_obj)
{
	if (index >= 0 and index < AsConfig::Max_Modules_Count)
		Modules[index++] = game_obj;
}
//------------------------------------------------------------------------------------------------------------