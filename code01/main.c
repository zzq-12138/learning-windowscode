#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MessageBox(NULL, TEXT("这是我在鱼小C学的第二个程序!"), TEXT("打招呼"), MB_ABORTRETRYIGNORE);
	return 0;
}