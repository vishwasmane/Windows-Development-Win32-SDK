// Headers
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include "MyWindow.h"

// 1. Declare FILE pointer
FILE *vmGpFile = NULL;
HWND ghwnd, ghwndPaintView;

// global callback function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
BOOL CALLBACK EnumChildWindowsProc(HWND, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	// 2. Create/Open log file in write mode.
	if (fopen_s(&vmGpFile, "VNMLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Can not create desired file."), TEXT("ERROR"), MB_OK);
		exit(0);
	}

	// code
	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register above class 
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szAppName,
		TEXT("Windows SDK Window : Vishwas N. Mane - Notepad Hacker"),
		WS_OVERLAPPEDWINDOW,//Window style
		CW_USEDEFAULT,//Create Window X pos
		CW_USEDEFAULT,//Create Window Y pos
		400,//Create Window Width of window
		400,//Create Window Height of window
		NULL,//Parent window handle - HWND_DESKTOP or NULL same
		NULL,//Window Menu(like File Menu) handle
		hInstance,//my HINSTANCE given by OS
		NULL);//Any extra info? _FAR void* - can be of any type.

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 4. Close FILE pointer after use.
	fclose(vmGpFile);

	// 5. Finally Set FILE pointer to NULL.
	vmGpFile = NULL;

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//Variable declarations
	HDC hdc;
	RECT rc;
	BOOL result;
	static HICON hIcon;
	static HINSTANCE notepadWindowInstance;
	TCHAR notepadText[256] = TEXT("This text is added using a hack....");
	
	// code 
	switch (iMsg)
	{
	case WM_CREATE:
		notepadWindowInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hIcon = LoadIcon(notepadWindowInstance, MAKEINTRESOURCE(MYICON));
		if (hIcon != NULL)
		{
			//MessageBox(hwnd, TEXT("Icon loaded"), TEXT("Icon loaded"), MB_OK);
		}
		
		// 3. Write to file
		fprintf(vmGpFile, "In WM_Create.\n");
		break;

	case WM_CHAR: // If we use WM_KEYDOWN, we will have to use hex values of characters.
		switch (wParam)
		{
		case 'h': // hack - find handle for the app window to be hacked
			EnumWindows(EnumWindowsProc, NULL);
			EnumChildWindows(ghwnd, EnumChildWindowsProc, NULL);
			break;
		
		case 'i': // change icon
			SendMessage(ghwnd, WM_SETICON, 0, (LPARAM)hIcon);
			MessageBox(hwnd, TEXT("Notepad Icon changed successfully!"), TEXT("Notepad Icon changed"), MB_OK);
			break;

		case 't': // change content or text
			for (size_t i = 0; i < strlen(notepadText); i++)
			{
				SendMessage(ghwndPaintView, WM_CHAR, (WPARAM)notepadText[i], 1);
			}
			break;
		
		case 'd': // delte menu
			SetMenu(ghwnd, NULL);
			break;

		case 'm': // minimize
			ShowWindow(ghwnd, SW_MINIMIZE);
			break;
		
		case 'c': // window text
			SetWindowText(ghwnd, TEXT("This is hacked title...."));
			break;

		case 'k': // kill application
			SendMessage(ghwnd, WM_CLOSE, 0, 0);
			break;

		default:
			break;
		}
		
		break;

	case WM_PAINT:
		break;

	case WM_DESTROY:
		// 3. Write to file
		fprintf(vmGpFile, "In WM_DESTROY.\n");
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	// variable declarations
	char className[250];
	int classNameLength = 0;
	//FILE* fp = NULL;

	// code
	//fopen_s(&fp, "ClassNames.txt", "a+");

	classNameLength = GetClassName(hwnd, className, 250);
	if (classNameLength > 0)
	{
		//fprintf(fp, "ClassName = %s\n", className);
	}
	
	// get notepad window handle
	if(strncmp(className, "Notepad", 7) == 0)
	{
		ghwnd = hwnd;
		MessageBox(NULL, TEXT("Notepad hack successfull!!!!"), TEXT("Notepad window is hacked now"), MB_OK);
		//fclose(fp);
		//fp = NULL;
		// stop EnumWindowProc
		return FALSE;
	}

	//fclose(fp);
	//fp = NULL;

	// keep iterating all present windows
	return TRUE;
}

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
{
	char className[250];
	int classNameLength = 0;
	//FILE* fp = NULL;

	// code
	//fopen_s(&fp, "ChildWindowClassNames.txt", "a+");
	classNameLength = GetClassName(hwnd, className, 250);
	if (classNameLength > 0)
	{
		//fprintf(fp, "ClassName = %s\n", className);
	}
	
	// get notepad window handle
	if(strncmp(className, "Edit", 4) == 0)
	{
		ghwndPaintView = hwnd;
		MessageBox(NULL, TEXT("Notepad edit child window hacked!!!!"), TEXT("Notepad edit child window hacked"), MB_OK);
		//fclose(fp);
		//fp = NULL;
		// stop EnumWindowProc
		return FALSE;
	}

	//fclose(fp);
	//fp = NULL;

	// keep iterating all present windows
	return TRUE;
}
