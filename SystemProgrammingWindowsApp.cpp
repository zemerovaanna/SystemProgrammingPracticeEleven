#include "framework.h"
#include "SystemProgrammingWindowsApp.h"
#include <commctrl.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SYSTEMPROGRAMMINGWINDOWSAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSTEMPROGRAMMINGWINDOWSAPP));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTEMPROGRAMMINGWINDOWSAPP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SYSTEMPROGRAMMINGWINDOWSAPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

#define IDC_TABCONTROL 100
#define IDC_PROGRESS 101
#define IDC_EDIT 102
#define IDC_SPIN 103
#define IDC_INCREMENT_BUTTON 114
#define IDC_DECREMENT_BUTTON 115

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}

	HWND hWndTabControl = CreateWindowEx(0, WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | TCS_FIXEDWIDTH, 400, 10, 400, 300, hWnd, (HMENU)IDC_TABCONTROL, GetModuleHandle(NULL), NULL);
	if (!hWndTabControl)
	{
		return FALSE;
	}

	TCITEM tie;
	tie.mask = TCIF_TEXT;

	const wchar_t* tabText1 = L"Tab 1";
	tie.pszText = const_cast<LPWSTR>(tabText1);
	TabCtrl_InsertItem(hWndTabControl, 0, &tie);

	const wchar_t* tabText2 = L"Tab 2";
	tie.pszText = const_cast<LPWSTR>(tabText2);
	TabCtrl_InsertItem(hWndTabControl, 1, &tie);

	const wchar_t* tabText3 = L"Tab 3";
	tie.pszText = const_cast<LPWSTR>(tabText3);
	TabCtrl_InsertItem(hWndTabControl, 2, &tie);

	HWND hWndProgressBar = CreateWindowEx(0, PROGRESS_CLASS, L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 150, 20, hWnd, (HMENU)IDC_PROGRESS, GetModuleHandle(NULL), NULL);
	if (!hWndProgressBar) {
		return FALSE;
	}

	SendMessage(hWndProgressBar, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, 100));
	SendMessage(hWndProgressBar, PBM_SETPOS, 0, 0);

	HWND hWndIncrementButton = CreateWindowEx(0, L"BUTTON", L"Increment", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 180, 10, 80, 30, hWnd, (HMENU)IDC_INCREMENT_BUTTON, GetModuleHandle(NULL), NULL);
	if (!hWndIncrementButton)
	{
		return FALSE;
	}

	HWND hWndDecrementButton = CreateWindowEx(0, L"BUTTON", L"Decrement", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 180, 50, 80, 30, hWnd, (HMENU)IDC_DECREMENT_BUTTON, GetModuleHandle(NULL), NULL);
	if (!hWndDecrementButton)
	{
		return FALSE;
	}

	HWND hWndEdit = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 100, 150, 30, hWnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);
	if (!hWndEdit)
	{
		return FALSE;
	}

	SetWindowText(hWndEdit, L"Edit me!");

	HWND hWndSpin = CreateWindowEx(0, L"msctls_updown32", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 200, 150, 50, hWnd, (HMENU)IDC_SPIN, GetModuleHandle(NULL), NULL);
	if (!hWndSpin)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

TCHAR szFile[MAX_PATH];
wchar_t szFindWhat[MAX_PATH];
wchar_t szReplaceWith[MAX_PATH];
OPENFILENAME ofn;
FINDREPLACE fr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)
			{
				case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;

				case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

				case IDM_OPEN:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
				if (GetOpenFileName(&ofn) == TRUE)
				{
					MessageBox(hWnd, szFile, L"File Selected", MB_OK);
				}
				break;

				case IDM_REPLACE:
				ZeroMemory(&fr, sizeof(fr));
				fr.lStructSize = sizeof(fr);
				fr.hwndOwner = hWnd;
				fr.wFindWhatLen = sizeof(szFindWhat);
				fr.wReplaceWithLen = sizeof(szReplaceWith);
				fr.Flags = FR_DOWN;
				fr.lpstrFindWhat = szFindWhat;
				fr.lpstrReplaceWith = szReplaceWith;
				if (ReplaceText(&fr) != FALSE) {

				}
				break;

				case IDC_INCREMENT_BUTTON:
				SendMessage(GetDlgItem(hWnd, IDC_PROGRESS), PBM_DELTAPOS, 10, 0);
				break;

				case IDC_DECREMENT_BUTTON:
				SendMessage(GetDlgItem(hWnd, IDC_PROGRESS), PBM_DELTAPOS, -10, 0);
				break;

				default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
		PostQuitMessage(0);
		break;

		case WM_MBUTTONDBLCLK:
		MessageBox(hWnd, L"Колёсико мыши нажато дважды", L"WM_MBUTTONDBLCLK Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_MBUTTONDOWN:
		MessageBox(hWnd, L"Колёсико мыши нажато", L"WM_MBUTTONDOWN Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_MBUTTONUP:
		MessageBox(hWnd, L"Колёсико мыши освобождено", L"WM_MBUTTONUP Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_MOUSEWHEEL:
		MessageBox(hWnd, L"Колёсико мыши повернулось", L"WM_MOUSEWHEEL Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_SIZING:
		MessageBox(hWnd, L"Изменение размера окна", L"WM_SIZING Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_SYSCHAR:
		MessageBox(hWnd, L"Системная кнопка нажата с другой кнопкой", L"WM_SYSCHAR Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		case WM_SYSKEYUP:
		MessageBox(hWnd, L"Системная кнопка освобождена", L"WM_SYSKEYUP Triggered", MB_OK | MB_ICONINFORMATION);
		break;

		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
		return (INT_PTR)TRUE;

		case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}