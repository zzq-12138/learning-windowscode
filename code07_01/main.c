/*
	要求一：垂直和水平居中显示两行字符串
	要求二：FishC 和 WWDC 第一个字母显示下划线
	要求三：当窗口尺寸小于字符串时，自动截除“越界”部分并替换为省略号“…”
*/
//
// Created by 81201 on 2024/1/20.
//
#include <windows.h>
#include "windowsx.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	SetConsoleOutputCP(CP_UTF8);
	system("chcp 65001");
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
	static RECT QRect;
	int xPos, yPos;

	QRect.left = 100;
	QRect.top = 100;
	QRect.right = 300;
	QRect.bottom = 300;

	static TCHAR szContent2[100];
	const static TCHAR* szContent = TEXT("猜猜怎么退出，关闭按钮不在右上角哦！");
	const static TCHAR* szCaptainNQ = TEXT("警告");
	const static TCHAR* szContentNQ = TEXT("说了，关闭按钮在窗口中...先找找看吧！");
	const static TCHAR* szCaptainQ = TEXT("恭喜！");
	const static TCHAR* szContentQ = TEXT("你找到了退出方法，真的要退出吗？");
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		// 计算字符串的宽度和高度，但不显示
		DrawText(hdc, szContent, -1, &rect,  DT_CALCRECT | DT_WORD_ELLIPSIS | DT_CENTER );

		// 调整rect以使文本在窗口中居中
		int textWidth = rect.right - rect.left;
		int textHeight = rect.bottom - rect.top;
		GetClientRect(hwnd, &rect); // 重新获取客户区大小
		rect.left = (rect.right / 2) - (textWidth / 2);
		rect.top = (rect.bottom / 2) - (textHeight / 2);
		rect.right = rect.left + textWidth;
		rect.bottom = rect.top + textHeight;

		// 实际绘制文本
		DrawText(hdc, szContent, -1, &rect, DT_WORD_ELLIPSIS | DT_CENTER);

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		MessageBox(hwnd,szContentNQ ,szCaptainNQ , MB_OK);
		// Else: User canceled. Do nothing.
		return 0;
	case WM_RBUTTONUP:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		POINT pt = {xPos,yPos} ;
		if(PtInRect(&QRect, pt))
		{
			if (MessageBox(hwnd, szContentQ, szCaptainQ, MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hwnd);
			}
		}
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}
//
// Created by 81201 on 24-7-9.
//
