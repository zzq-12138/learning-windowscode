#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	TCHAR *szCaptain = TEXT("呜呜呜");
	static TCHAR szContent[] = TEXT("你真的忍心把我关掉吗？？");
	while (IDNO == MessageBox(NULL, szContent, szCaptain,
		MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2));
	return 0;
}