#pragma once
#include "resource.h"
#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
class AsFrame_DC
{
public:
	~AsFrame_DC();
	AsFrame_DC();
	HDC Get_DC(HWND hwnd, HDC hdc);
	void Delete_DC();

	int Width, Height;

private:
	HDC DC;
	HBITMAP Bitmap;
};
//------------------------------------------------------------------------------------------------------------
class AsMain_Window
{
public:
	AsMain_Window();
	int Main(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR command_line, int command_show);

	AsEngine Engine;
	HINSTANCE Instance;

private:
	ATOM Register_Class();
	BOOL Init_Instance(int command_show);
	void On_Paint(HWND hwnd);
	static LRESULT CALLBACK Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static AsMain_Window *Window;
	static const int Max_String_Size = 100;
	AsFrame_DC Frame_DC;
	WCHAR szTitle[Max_String_Size];
	WCHAR szWindowClass[Max_String_Size];
};
//------------------------------------------------------------------------------------------------------------