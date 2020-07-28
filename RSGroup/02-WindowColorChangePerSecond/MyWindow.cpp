// Headers
#include<windows.h>
#include<stdio.h>

#define MYTIMER 111

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
		TEXT("Windows SDK Window : Vishwas N. Mane - Paint - Window Color animation"),
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
	HDC hdc;
	static RECT rc;
	PAINTSTRUCT ps;
	static HBRUSH hBrushes[6];
	static int brushIndex;

	// code 
	switch (iMsg)
	{
	case WM_CREATE:
		// 3. Write to file
		fprintf(vmGpFile, "In WM_Create.\n");

		hBrushes[0] = CreateSolidBrush(RGB(255, 0, 0));
		hBrushes[1] = CreateSolidBrush(RGB(0, 255, 0));
		hBrushes[2] = CreateSolidBrush(RGB(0, 0, 255));
		hBrushes[3] = CreateSolidBrush(RGB(0, 255, 255));
		hBrushes[4] = CreateSolidBrush(RGB(255, 0, 255));
		hBrushes[5] = CreateSolidBrush(RGB(255, 255, 0));
		brushIndex = 0;

		SetTimer(hwnd, MYTIMER, 1000, NULL); // Last parameter NULL - No timer callback only then it sends WM_TIMER message
		break;

	case WM_TIMER:
		KillTimer(hwnd, MYTIMER);

		if(brushIndex >= 6)
			brushIndex = 0;
		
		InvalidateRect(hwnd, &rc, TRUE);
		brushIndex++;
		SetTimer(hwnd, MYTIMER, 1000, NULL);
		
		break;
	
	case WM_PAINT:
		GetClientRect(hwnd, &rc);

		hdc = BeginPaint(hwnd, &ps);

		//SetBkColor(hdc, RGB(0, 0, 0));//background color of Text/ or whatever we are going to do drawing
		
		

		FillRect(hdc, &rc, hBrushes[brushIndex]);
	
		EndPaint(hwnd, &ps); // stop painting
		
		break;

	case WM_DESTROY:

		brushIndex = 0;
		while(brushIndex < 6)
		{
			DeleteObject(hBrushes[brushIndex]);
			brushIndex++;
		}	

		// 3. Write to file
		fprintf(vmGpFile, "In WM_DESTROY.\n");
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
