// Headers
#include<windows.h>
#include<stdio.h>

// 1. Declare FILE pointer
FILE *vmGpFile = NULL;

// global callback function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
		TEXT("Windows SDK Window : Vishwas N. Mane - Paint - Color Palette"),
		WS_OVERLAPPEDWINDOW,//Window style
		CW_USEDEFAULT,//Create Window X pos
		CW_USEDEFAULT,//Create Window Y pos
		CW_USEDEFAULT,//Create Window Width of window
		CW_USEDEFAULT,//Create Window Height of window
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
	HDC hdc; // Handle to the Device Context - Graphic Card cha jankar or Duva between GC & OS.
	//PAINTSTRUCT ps;
	static RECT rc, rc1, rc2, rc3, rc4;
	
	TCHAR str[] = TEXT("Hello World !!!!");
	static COLORREF color = RGB(0, 255, 0);

	// code 
	switch (iMsg)
	{
	case WM_CREATE:
		// 3. Write to file
		fprintf(vmGpFile, "In WM_Create.\n");
		break;

	case WM_CHAR: // If we use WM_KEYDOWN, we will have to use hex values of characters.
		switch (wParam)
		{
		case 'r':
		case 'R':
			color = RGB(255, 0, 0);
			InvalidateRect(hwnd, &rc, FALSE); // Only update rect regin on window, Do not update background of rect.
			break;
		
		case 'g':
		case 'G':
			color = RGB(0, 255, 0);
			InvalidateRect(hwnd, &rc, FALSE);
			break;

		case 'b':
		case 'B':
			color = RGB(0, 0, 255);
			InvalidateRect(hwnd, &rc, FALSE);
			break;
		
		case 'c': // cyan
		case 'C':
			color = RGB(0, 255, 255);
			InvalidateRect(hwnd, &rc, FALSE); 
			break;

		case 'm': // magenta
		case 'M':
			color = RGB(255, 0, 255);
			InvalidateRect(hwnd, &rc, FALSE);
			break;
		
		case 'y': // yellow
		case 'Y':
			color = RGB(255, 255, 0);
			InvalidateRect(hwnd, &rc, FALSE);
			break;

		default:
			break;
		}
		
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);

		rc1.left = rc.left;
		rc1.top = rc.top;
		rc1.right = (rc.right - rc.left)/4;
		rc1.bottom = rc.bottom; 

		rc2.left = rc1.right;
		rc2.top = rc1.top;
		rc2.right = 2*(rc.right - rc.left)/4;
		rc2.bottom = rc.bottom; 

		rc3.left = rc2.right;
		rc3.top = rc2.top;
		rc3.right = 3*(rc.right - rc.left)/4;
		rc3.bottom = rc.bottom;

		rc4.left = rc3.right;
		rc4.top = rc3.top;
		rc4.right = 4*(rc.right - rc.left)/4;
		rc4.bottom = rc.bottom;

		hdc = GetDC(hwnd);//BeginPaint(hwnd, &ps);
		SetBkColor(hdc, RGB(0, 0, 0));//background color of Text/ or whatever we are going to do drawing
		
		FillRect(hdc, &rc1, CreateSolidBrush(RGB(255, 0, 0)));
		FillRect(hdc, &rc2, CreateSolidBrush(RGB(0, 255, 0)));
		FillRect(hdc, &rc3, CreateSolidBrush(RGB(0, 0, 255)));
		FillRect(hdc, &rc4, CreateSolidBrush(RGB(255, 255, 0)));

		ReleaseDC(hwnd, hdc);//EndPaint(hwnd, &ps); // stop painting
		
		break;

	case WM_DESTROY:
		// 3. Write to file
		fprintf(vmGpFile, "In WM_DESTROY.\n");
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
