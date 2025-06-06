/*
	要求一：垂直和水平居中显示两行字符串
	要求二：FishC 和 WWDC 第一个字母显示下划线
	要求三：当窗口尺寸小于字符串时，自动截除“越界”部分并替换为省略号“…”
*/
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
	static TCHAR szContent2[100];
	const static TCHAR* szContent = TEXT("大家好，这是周正奇的第一个窗口程序！");
	const static TCHAR* szCaptain = TEXT("大家好");
	const static TCHAR* szCaptainQ = TEXT("退出");
	const static TCHAR* szContentQ = TEXT("真的要退出吗？");
	const static TCHAR* szContent3 = TEXT("Hiding from the rain and snow. &FishC.com\n"
										  "Trying to forget but I won't let go.&WWDC");
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		// 计算字符串的宽度和高度，但不显示
		DrawText(hdc, szContent3, -1, &rect,  DT_CALCRECT | DT_WORD_ELLIPSIS | DT_CENTER );

		// 调整rect以使文本在窗口中居中
		int textWidth = rect.right - rect.left;
		int textHeight = rect.bottom - rect.top;
		GetClientRect(hwnd, &rect); // 重新获取客户区大小
		rect.left = (rect.right / 2) - (textWidth / 2);
		rect.top = (rect.bottom / 2) - (textHeight / 2);
		rect.right = rect.left + textWidth;
		rect.bottom = rect.top + textHeight;

		// 实际绘制文本
		DrawText(hdc, szContent3, -1, &rect, DT_WORD_ELLIPSIS | DT_CENTER);

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
