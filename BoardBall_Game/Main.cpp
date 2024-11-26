#include "framework.h"
#include "Main.h"

//#define MAX_LOADSTRING 100

//------------------------------------------------------------------------------------------------------------
AsMain_Window Main_Window;
AsMain_Window *AsMain_Window::Window = 0;
//------------------------------------------------------------------------------------------------------------




//AsFrame_DC
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	DeleteObject(Bitmap);
	DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
: DC(0), Bitmap(0), Width(0), Height(0) 
{
}
//------------------------------------------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	RECT rect;
	int dc_width, dc_height;

	GetClientRect(hwnd, &rect);

	dc_width = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	Width = dc_width;
	Height = dc_height;

	DC = CreateCompatibleDC(hdc);
	Bitmap = CreateCompatibleBitmap(hdc, Width, Height);

	SelectObject(DC, Bitmap);

	++rect.right;
	++rect.bottom;

	AsTools::Rect(DC, rect, AsConfig::BG_Color);
	return DC;
}
//------------------------------------------------------------------------------------------------------------
void AsFrame_DC::Delete_DC()
{
	DeleteObject(Bitmap);
	DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------



//Windows.h
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Main_Window.Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




//AsMain_Window
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
: Instance(0), Engine(), Frame_DC(), szTitle{}, szWindowClass{}
{
	Window = this;
}
//------------------------------------------------------------------------------------------------------------
int AsMain_Window::Main(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR command_line, int command_show)
{
	MSG msg;
	HACCEL accel_table;

	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(command_line);

	Instance = instance;

	LoadStringW(instance, IDS_APP_TITLE, szTitle, Max_String_Size);
	LoadStringW(instance, IDC_BOARDBALLGAME, szWindowClass, Max_String_Size);
	Register_Class();

	if (! Init_Instance(command_show) )
		return FALSE;

	accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_BOARDBALLGAME));


	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_BOARDBALLGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BOARDBALLGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
BOOL AsMain_Window::Init_Instance(int command_show)
{
	HWND hwnd;
	RECT window_rect {};

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, Instance, nullptr);

	if (hwnd == 0)
		return FALSE;

	Engine.Init_Engine(hwnd);

	ShowWindow(hwnd, command_show);
	UpdateWindow(hwnd);

	return TRUE;
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc, frame_dc;

	hdc = BeginPaint(hwnd, &ps);

	frame_dc = Frame_DC.Get_DC(hwnd, hdc);
	
	Engine.Draw_Frame(frame_dc, ps.rcPaint);

	BitBlt(hdc, 0, 0, Frame_DC.Width, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	Frame_DC.Delete_DC();
	
	EndPaint(hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Window->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:

		Window->On_Paint(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Left, true);

		case VK_RIGHT:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Right, true);

		case VK_SPACE:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Space, true);
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Left, false);

		case VK_RIGHT:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Right, false);

		case VK_SPACE:
			return Window->Engine.On_Key(AsEngine::EKey_Type::Space, false);
		}
		break;

	case WM_TIMER:
		if (wParam == Window->Engine.Timer_ID)
			return Window->Engine.On_Timer();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK AsMain_Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------