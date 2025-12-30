#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

// Global Variables
const wchar_t CLASS_NAME[] = L"EdgeLightWindowClass";

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{
	// Register the window class
	WNDCLASS wc = {};

	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	// Create the window
	HWND hwnd = CreateWindowEx(
		WS_EX_TOPMOST |			// always on top
		WS_EX_LAYERED |			// allows transparency
		WS_EX_TRANSPARENT |		// click-through
		WS_EX_TOOLWINDOW,		// no taskbar

		CLASS_NAME,
		L"",					// No title
		WS_POPUP,				// borderless window

		// Size and position
		0, 0,							// position (top-left)
		GetSystemMetrics(SM_CXSCREEN),	// Fullscreen width
		GetSystemMetrics(SM_CYSCREEN),	// Fullscreen height

		NULL,		// Parent window
		NULL,		// Menu
		hInstance,	// Instance handle
		NULL		// Additional application data
	);

	if (!hwnd) 
		return 0;

	// Make window fully transparent (alpha = 255 means visible content allowed)
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

	// Show the window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_ERASEBKGND:
		return 1; // Prevent windows from erasing background (black fill)

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT rect;
		GetClientRect(hwnd, &rect);

		// Create a blue brush (fake "glow")
		HBRUSH brush = CreateSolidBrush(RGB(0, 120, 255));

		// Top edge
		RECT top = { rect.left, rect.top, rect.right, rect.top + 5 };
		FillRect(hdc, &top, brush);

		// Bottom edge
		RECT bottom = { rect.left, rect.bottom - 5, rect.right, rect.bottom };
		FillRect(hdc, &bottom, brush);

		// Left edge
		RECT left = { rect.left, rect.top, rect.left + 5, rect.bottom };
		FillRect(hdc, &left, brush);

		// Right edge
		RECT right = { rect.right - 5, rect.top, rect.right, rect.bottom };
		FillRect(hdc, &right, brush);

		DeleteObject(brush);
		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}