#include <windows.h>

LRESULT CALLBACK WndProc(HWND _v0, UINT _v1, WPARAM _v2, LPARAM _v3) {
	if (_v1		== WM_POWERBROADCAST && _v2 == PBT_POWERSETTINGCHANGE) {
			if (!memcmp(&((POWERBROADCAST_SETTING*)_v3)->PowerSetting, &GUID_LIDSWITCH_STATE_CHANGE, sizeof(GUID))) {
				if (!*((POWERBROADCAST_SETTING*)_v3)->Data) LockWorkStation();
			}
	}
	return DefWindowProc(_v0, _v1, _v2, _v3);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	HANDLE v0;
	HWND v1;
	MSG v2;
	WNDCLASS v3;
	HPOWERNOTIFY v4;
	TCHAR v5[] = L"LidLock";

	v0 = CreateMutex(0, 0, L"VT_TL_LidLock");
	if (GetLastError() == ERROR_ALREADY_EXISTS) return -1;

	v3.lpszClassName = v5;
	v3.hInstance = hInstance;
	v3.lpfnWndProc = WndProc;
	v3.lpszMenuName = 0;
	v3.cbWndExtra = 0;
	v3.cbClsExtra = 0;
	v3.style = 0;
	v3.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	v3.hIcon = LoadIcon(0, IDI_APPLICATION);
	v3.hCursor = LoadCursor(0, IDC_ARROW);

	if (!RegisterClass(&v3)){
		MessageBox(0, L"Failed to register a window class.", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	if ((v1 = CreateWindow(v5, L"LidLock", 0, 0, 0, 0, 0, 0, 0, hInstance, 0)) == 0) {
		MessageBox(0, L"Failed to create a window.", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	if ((v4 = RegisterPowerSettingNotification(v1, &GUID_LIDSWITCH_STATE_CHANGE, 0)) == 0) {
		MessageBox(0, L"Failed to register power change notification.", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	while (GetMessage(&v2, 0, 0, 0)){
		TranslateMessage(&v2);
		DispatchMessage(&v2);
	}
	return 0;
}

