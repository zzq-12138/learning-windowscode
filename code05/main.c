//
// Created by 81201 on 2024/1/20.
//
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MyWindow");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,
		TEXT("周正奇的第一个窗口程序"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(NULL, TEXT("GetMessage Error!"), szAppName, MB_ICONERROR);
			return 0;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int n = 0;
	static TCHAR szContent2[100];
	const static TCHAR* szContent = TEXT("大家好，这是周正奇的第一个窗口程序！");
	const static TCHAR* szCaptain = TEXT("大家好");
	const static TCHAR* szCaptainQ = TEXT("退出");
	const static TCHAR* szContentQ = TEXT("真的要退出吗？");
	const static TCHAR* szContent3 = TEXT("Hiding from the rain and snow.\n"
										  "Trying to forget but I won't let go.\n"
										  "Looking at a crowded street.\n"
										  "Listening to my own heart beat.\n"
										  "So many people all around the world.\n"
										  "Tell me where do I find someone like you girl.");
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		if (n == 0)
		{
			DrawText(hdc, szContent3, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		else
		{
			wsprintf(szContent2, TEXT("我被重绘了%d次"), n);
			DrawText(hdc, szContent2, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		++n;
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, szContentQ, szCaptainQ, MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		// Else: User canceled. Do nothing.
		return 0;
	case WM_LBUTTONUP:
		if (MK_RBUTTON == wParam)
		{
			MessageBox(hwnd, TEXT("左右键同时摁下"), szCaptain, MB_OK);
		}
		return 0;
	case WM_RBUTTONUP:
		if (MK_LBUTTON == wParam)
		{
			MessageBox(hwnd, TEXT("左右键同时摁下"), szCaptain, MB_OK);
		}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}
