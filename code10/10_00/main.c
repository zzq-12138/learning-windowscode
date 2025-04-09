//
// Created by 81201 on 24-7-11.
//
//在屏幕中间打印一个由 x 构成的三角形

#include <windows.h>
#include "strsafe.h"

#define N 5

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
	size_t iTarget;

	static int cxChar, cyChar;

	TEXTMETRIC tm;
	static int n = 0;
	static TCHAR szContent2[100];
	const static TCHAR* szContent = TEXT("大家好，这是周正奇的第一个窗口程序！");
	const static TCHAR* szCaptain = TEXT("大家好");
	const static TCHAR* szCaptainQ = TEXT("退出");
	const static TCHAR* szContentQ = TEXT("真的要退出吗？");

	static TCHAR szBuffer[100];

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		for (int i = 1; i <= N; i++)
		{
			// 清空缓冲区
			StringCchCopy(szBuffer, 100, TEXT(""));
			for (int j = 0; j < 2 * i - 1; j++)
			{
				// 打印一个 x 到缓冲区
				StringCchCat(szBuffer, 100, TEXT("x"));

			}
			// 将缓冲区中的一行 x 显示到屏幕上，请思考：如何显示到屏幕中间？
			StringCchLength(szBuffer, 100, &iTarget);

			SetTextAlign(hdc, GetTextAlign(hdc) | TA_CENTER);
			TextOut(hdc, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 - (N/2 - i + 1.5) * cyChar, szBuffer, iTarget);
		}

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
