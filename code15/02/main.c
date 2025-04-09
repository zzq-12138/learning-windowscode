#include <windows.h>
#include <vector>
#include <fstream>
#include "strsafe.h"

#define ID_SAVE 1
#define ID_LOAD 2

enum ShapeType { LINE, RECTANGLE, CIRCLE };

struct Shape {
	ShapeType type;
	POINT start;
	POINT end;
};

std::vector<Shape> shapes;
Shape* selectedShape = nullptr;
POINT lastMousePos;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SaveShapesToFile(const char* filename);
void LoadShapesFromFile(const char* filename);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("DrawBoard");
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
		TEXT("画板程序"),
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

void DrawShape(HDC hdc, const Shape& shape)
{
switch (shape.type)
{
case LINE:
MoveToEx(hdc, shape.start.x, shape.start.y, NULL);
LineTo(hdc, shape.end.x, shape.end.y);
break;
case RECTANGLE:
Rectangle(hdc, shape.start.x, shape.start.y, shape.end.x, shape.end.y);
break;
case CIRCLE:
Ellipse(hdc, shape.start.x, shape.start.y, shape.end.x, shape.end.y);
break;
}
}

Shape* HitTest(POINT pt)
{
	for (auto& shape : shapes)
	{
		switch (shape.type)
		{
		case LINE:
			// 简单的线段点击测试
			if (abs((shape.end.y - shape.start.y) * pt.x - (shape.end.x - shape.start.x) * pt.y + shape.end.x * shape.start.y - shape.end.y * shape.start.x) < 1000)
				return &shape;
			break;
		case RECTANGLE:
			if (pt.x >= shape.start.x && pt.x <= shape.end.x && pt.y >= shape.start.y && pt.y <= shape.end.y)
				return &shape;
			break;
		case CIRCLE:
			int radius = (shape.end.x - shape.start.x) / 2;
			POINT center = { (shape.start.x + shape.end.x) / 2, (shape.start.y + shape.end.y) / 2 };
			if ((pt.x - center.x) * (pt.x - center.x) + (pt.y - center.y) * (pt.y - center.y) <= radius * radius)
				return &shape;
			break;
		}
	}
	return nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT startPoint;
	static bool isDrawing = false;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		CreateWindow(TEXT("button"), TEXT("保存"), WS_CHILD | WS_VISIBLE, 10, 10, 50, 30, hwnd, (HMENU)ID_SAVE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("button"), TEXT("加载"), WS_CHILD | WS_VISIBLE, 70, 10, 50, 30, hwnd, (HMENU)ID_LOAD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SAVE:
			SaveShapesToFile("shapes.dat");
			break;
		case ID_LOAD:
			LoadShapesFromFile("shapes.dat");
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		return 0;

	case WM_LBUTTONDOWN:
		startPoint.x = LOWORD(lParam);
		startPoint.y = HIWORD(lParam);
		selectedShape = HitTest(startPoint);
		if (selectedShape)
		{
			lastMousePos = startPoint;
		}
		else
		{
			isDrawing = true;
		}
		return 0;

	case WM_MOUSEMOVE:
		if (isDrawing)
		{
			hdc = GetDC(hwnd);
			SetROP2(hdc, R2_NOTXORPEN);
			if (shapes.size() > 0)
			{
				DrawShape(hdc, shapes.back());
			}
			Shape newShape = { RECTANGLE, startPoint, { LOWORD(lParam), HIWORD(lParam) } };
			shapes.back() = newShape;
			DrawShape(hdc, newShape);
			ReleaseDC(hwnd, hdc);
		}
		else if (selectedShape)
		{
			POINT currentPos = { LOWORD(lParam), HIWORD(lParam) };
			int dx = currentPos.x - lastMousePos.x;
			int dy = currentPos.y - lastMousePos.y;
			selectedShape->start.x += dx;
			selectedShape->start.y += dy;
			selectedShape->end.x += dx;
			selectedShape->end.y += dy;
			lastMousePos = currentPos;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;

	case WM_LBUTTONUP:
		if (isDrawing)
		{
			isDrawing = false;
			Shape newShape = { RECTANGLE, startPoint, { LOWORD(lParam), HIWORD(lParam) } };
			shapes.push_back(newShape);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		selectedShape = nullptr;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (const auto& shape : shapes)
		{
			DrawShape(hdc, shape);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void SaveShapesToFile(const char* filename)
{
	std::ofstream ofs(filename, std::ios::binary);
	size_t size = shapes.size();
	ofs.write((char*)&size, sizeof(size));
	for (const auto& shape : shapes)
	{
		ofs.write((char*)&shape, sizeof(shape));
	}
}

void LoadShapesFromFile(const char* filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	size_t size;
	ifs.read((char*)&size, sizeof(size));
	shapes.resize(size);
	for (auto& shape : shapes)
	{
		ifs.read((char*)&shape, sizeof(shape));
	}
}