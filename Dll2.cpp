// Dll2.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"

#define PROCESS_NAME "notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHOOK = NULL;
HWND g_hWnd = NULL;

BOOL APIENTRY DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved) {
	printf("main called\n");
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		printf("Attached!!\n");
		g_hInstance = hinstDll; 
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	char szPath[MAX_PATH] = {0, };
	char * p = NULL;
	printf("KP called!!\n");
	if (nCode == 0) {
		if ( !(lParam & 0x80000000) ) {
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			p = strrchr(szPath, '\\');
			if (!_stricmp(p + 1, PROCESS_NAME)) {
				return 1;
			}
		}
	}
	return CallNextHookEx(g_hHOOK, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif 
__declspec(dllexport) void HookStart() 
{
	printf("hookstart called\n");
	printf("g_hook : %x\n", g_hHOOK);
	g_hHOOK = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
	printf("hookstart called\n"); 
	printf("g_hook : %x\n", g_hHOOK);
}
__declspec(dllexport) void HookStop() 
{
	if (g_hHOOK) {
		UnhookWindowsHookEx(g_hHOOK);
		g_hHOOK = NULL;
	}
}
#ifdef __cplusplus
}
#endif


