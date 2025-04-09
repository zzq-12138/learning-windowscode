#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawRect(HWND pHwnd, RECT rect, int i);

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
	RECT rect2 = { 200, 100, 300, 200 };
	const static TCHAR* szContent = TEXT("大家好，这是周正奇的第一个窗口程序！");
	const static TCHAR* szCaptain = TEXT("大家好");
	const static TCHAR* szCaptainQ = TEXT("退出");
	const static TCHAR* szContentQ = TEXT("真的要退出吗？");
	const static TCHAR* szCaptainQ1 = TEXT("无法退出");
	const static TCHAR* szContentQ1 = TEXT("此处无法退出，请寻找正确地方？");

	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawRect(hwnd, rect2, BLACK_BRUSH);
		DrawText(hdc, szContent, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		MessageBox(hwnd, szContentQ1, szCaptainQ1, MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT point = { x, y };
		if (PtInRect(&rect2, point))
		{
			if (MessageBox(hwnd, szContentQ, szCaptainQ, MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hwnd);
			}
		}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}
void DrawRect(HWND pHwnd, RECT rect, int i)
{
	HDC hdc = GetDC(pHwnd);
	HBRUSH hBrush = CreateSolidBrush(i);
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
	ReleaseDC(pHwnd, hdc);
}
