#include "Viewer.h"
#include <Windows.h>
#include <tchar.h>
#include <map>
#include <string>


SHAKURAS_BEGIN;


LRESULT ScreenEventProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class WindowsViewer : public Viewer, public std::enable_shared_from_this<WindowsViewer> {
public:
	WindowsViewer() {
		screen_w = screen_h = 0;
		screen_mx = screen_my = screen_mb = 0;
		screen_keys.assign(0);
		screen_close = false;
		screen_handle = NULL;
		screen_dc = NULL;
		screen_hb = NULL;
		screen_ob = NULL;
		screen_fb = nullptr;
		screen_pitch = 0;
	}

	virtual ~WindowsViewer() {}

public:
	virtual int initialize(int w, int h, const char* title) {
		WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)ScreenEventProc, 0, 0, 0,
			NULL, NULL, NULL, NULL, _T("SCREEN_GRPC3D") };
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4,
			0, 0, 0, 0, } };
		RECT rect = { 0, 0, w, h };
		int wx, wy, sx, sy;
		LPVOID ptr;
		HDC hDC;

		close();

		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		if (!RegisterClass(&wc)) {
			return -1;
		}

		screen_handle = CreateWindow(_T("SCREEN_GRPC3D"), title,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
		if (screen_handle == NULL) {
			return -2;
		}

		hDC = GetDC(screen_handle);
		screen_dc = CreateCompatibleDC(hDC);
		ReleaseDC(screen_handle, hDC);

		screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
		if (screen_hb == NULL) {
			return -3;
		}

		screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
		screen_fb = (unsigned char*)ptr;
		screen_w = w;
		screen_h = h;
		screen_pitch = w * 4;

		AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
		wx = rect.right - rect.left;
		wy = rect.bottom - rect.top;
		sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
		sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
		if (sy < 0) sy = 0;
		SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
		SetForegroundWindow(screen_handle);

		ShowWindow(screen_handle, SW_NORMAL);
		dispatch();

		screen_keys.assign(0);
		screen_close = false;
		memset(screen_fb, 0, w * h * 4);

		objects[screen_handle] = std::dynamic_pointer_cast<WindowsViewer>(shared_from_this());

		return 0;
	}

	virtual bool testUserMessage(UserMessage msg) {
		switch (msg) {
		case shakuras::kUMUp:
			return screen_keys[VK_UP] != 0;
		case shakuras::kUMDown:
			return screen_keys[VK_DOWN] != 0;
		case shakuras::kUMLeft:
			return screen_keys[VK_LEFT] != 0;
		case shakuras::kUMRight:
			return screen_keys[VK_RIGHT] != 0;
		case shakuras::kUMEsc:
			return screen_keys[VK_ESCAPE] != 0;
		case shakuras::kUMSpace:
			return screen_keys[VK_SPACE] != 0;
		case shakuras::kUMClose:
			return screen_close;
		default:
			return false;
		}
	}

	virtual void dispatch() {
		MSG msg;
		while (true) {
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				break;
			}
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			DispatchMessage(&msg);
		}
	}

	virtual void update(void) {
		HDC hDC = GetDC(screen_handle);
		BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
		ReleaseDC(screen_handle, hDC);
		dispatch();
	}

	virtual void* frameBuffer() {
		return screen_fb;
	}

	virtual int width() {
		return screen_w;
	}

	virtual int height() {
		return screen_h;
	}

	int close() {
		if (screen_dc) {
			if (screen_ob) {
				SelectObject(screen_dc, screen_ob);
				screen_ob = NULL;
			}
			DeleteDC(screen_dc);
			screen_dc = NULL;
		}
		if (screen_hb) {
			DeleteObject(screen_hb);
			screen_hb = NULL;
		}
		if (screen_handle) {
			CloseWindow(screen_handle);
			screen_handle = NULL;
		}
		return 0;
	}

	LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (hWnd == screen_handle) {
			switch (msg) {
			case WM_CLOSE: screen_close = true; return 1;
			case WM_KEYDOWN: screen_keys[wParam & 511] = 1; return 1;
			case WM_KEYUP: screen_keys[wParam & 511] = 0; return 1;
			}
		}
		return 0;
	}

public:
	int screen_w, screen_h;
	int screen_mx, screen_my, screen_mb;
	std::array<int, 512> screen_keys;
	bool screen_close;
	HWND screen_handle;
	HDC screen_dc;
	HBITMAP screen_hb;
	HBITMAP screen_ob;
	unsigned char* screen_fb;
	long screen_pitch;

public:
	static std::map<HWND, std::weak_ptr<WindowsViewer> > objects;
};


std::map<HWND, std::weak_ptr<WindowsViewer> > WindowsViewer::objects;


LRESULT ScreenEventProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	std::shared_ptr<WindowsViewer> winviewer = WindowsViewer::objects[hWnd].lock();
	if (winviewer) {
		LRESULT res = winviewer->onEvent(hWnd, msg, wParam, lParam);
		if (res != 0) {
			return res;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


ViewerPtr CreateGsViewer(std::string platform) {
	if (platform == "Windows") {
		return std::make_shared<WindowsViewer>();
	}
	return nullptr;
}


SHAKURAS_END;