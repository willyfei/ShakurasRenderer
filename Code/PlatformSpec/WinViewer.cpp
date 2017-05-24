#include "WinViewer.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
LRESULT ScreenEventProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


std::map<HWND, std::weak_ptr<WinViewer> > g_win_viewers;


LRESULT ScreenEventProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	std::shared_ptr<WinViewer> winviewer = g_win_viewers[hWnd].lock();
	if (winviewer) {
		LRESULT res = winviewer->onEvent(hWnd, msg, wParam, lParam);
		if (res != 0) {
			return res;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#elif defined(__ANDROID__)
#endif

WinViewer::WinViewer() {

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	hwnd_ = NULL;
#elif defined(__ANDROID__)
#endif

	width_ = height_ = 0;
	//keys_.assign(0);
	close_ = false;
	
}

bool WinViewer::testUserMessage(UserMessage msg) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	switch (msg) {
	case kUMUp:
		return keys_[VK_UP] != 0;
	case kUMDown:
		return keys_[VK_DOWN] != 0;
	case kUMLeft:
		return keys_[VK_LEFT] != 0;
	case kUMRight:
		return keys_[VK_RIGHT] != 0;
	case kUMEsc:
		return keys_[VK_ESCAPE] != 0;
	case kUMSpace:
		return keys_[VK_SPACE] != 0;
	case kUMClose:
		return close_;
	default:
		return false;
}
#elif defined(__ANDROID__)
#endif

	return false;
}

void WinViewer::dispatch() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
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
#elif defined(__ANDROID__)
#endif

	
}

int WinViewer::width() {
	return width_;
}

int WinViewer::height() {
	return height_;
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
LRESULT WinViewer::onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (hWnd == hwnd_) {
		switch (msg) {
		case WM_CLOSE: close_ = true; return 1;
		case WM_KEYDOWN: keys_[wParam & 511] = 1; return 1;
		case WM_KEYUP: keys_[wParam & 511] = 0; return 1;
		}
	}
	return 0;
}
#elif defined(__ANDROID__)
#endif

WinMemViewer::WinMemViewer() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	hdc_ = NULL;
	hbmp_ = NULL;
	org_hbmp_ = NULL;
#elif defined(__ANDROID__)
#endif

	
	frame_buffer_ = nullptr;
}

int WinMemViewer::initialize(int w, int h, const char* title) {

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)ScreenEventProc, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN_GRPC3D") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4UL,
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

	hwnd_ = CreateWindow(_T("SCREEN_GRPC3D"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (hwnd_ == NULL) {
		return -2;
	}

	hDC = GetDC(hwnd_);
	hdc_ = CreateCompatibleDC(hDC);
	ReleaseDC(hwnd_, hDC);

	hbmp_ = CreateDIBSection(hdc_, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (hbmp_ == NULL) {
		return -3;
	}

	org_hbmp_ = (HBITMAP)SelectObject(hdc_, hbmp_);
	frame_buffer_ = (unsigned char*)ptr;
	width_ = w;
	height_ = h;

	AdjustWindowRect(&rect, GetWindowLong(hwnd_, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(hwnd_, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(hwnd_);

	ShowWindow(hwnd_, SW_NORMAL);
	dispatch();

	keys_.assign(0);
	close_ = false;
	memset(frame_buffer_, 0, w * h * 4);

	g_win_viewers[hwnd_] = shared_from_this();
#elif defined(__ANDROID__)
#endif
	

	return 0;
}

void WinMemViewer::update(void) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	HDC hDC = GetDC(hwnd_);
	BitBlt(hDC, 0, 0, width_, height_, hdc_, 0, 0, SRCCOPY);
	ReleaseDC(hwnd_, hDC);
	dispatch();
#elif defined(__ANDROID__)
#endif
	
}

void* WinMemViewer::frameBuffer() {
	return frame_buffer_;
}

int WinMemViewer::close() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	if (hdc_) {
		if (org_hbmp_) {
			SelectObject(hdc_, org_hbmp_);
			org_hbmp_ = NULL;
		}
		DeleteDC(hdc_);
		hdc_ = NULL;
	}
	if (hbmp_) {
		DeleteObject(hbmp_);
		hbmp_ = NULL;
	}
	if (hwnd_) {
		CloseWindow(hwnd_);
		hwnd_ = NULL;
	}
#elif defined(__ANDROID__)
#endif
	
	return 0;
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

HGLRC InitGlContex(HDC hDC) {
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	BOOL res = SetPixelFormat(hDC, pixelFormat, &pfd);
	return wglCreateContext(hDC);
}

#elif defined(__ANDROID__)
#endif


WinRcViewer::WinRcViewer() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	hdc_ = NULL;
	hrc_ = NULL;
#elif defined(__ANDROID__)
#endif
	
}

int WinRcViewer::initialize(int w, int h, const char* title) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)ScreenEventProc, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN_GRPC3D") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4UL,
		0, 0, 0, 0, } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;

	close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) {
		return -1;
	}

	hwnd_ = CreateWindow(_T("SCREEN_GRPC3D"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (hwnd_ == NULL) {
		return -2;
	}

	hdc_ = GetDC(hwnd_);
	hrc_ = InitGlContex(hdc_);

	width_ = w;
	height_ = h;

	AdjustWindowRect(&rect, GetWindowLong(hwnd_, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(hwnd_, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(hwnd_);

	ShowWindow(hwnd_, SW_NORMAL);
	dispatch();

	keys_.assign(0);
	close_ = false;

	g_win_viewers[hwnd_] = shared_from_this();
#elif defined(__ANDROID__)
#endif

	return 0;
}

void WinRcViewer::update(void) {
	dispatch();
}

int WinRcViewer::close() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	if (hwnd_) {
		if (hdc_) {
			if (hrc_) {
				wglMakeCurrent(hdc_, NULL);
				wglDeleteContext(hrc_);
				hrc_ = NULL;
			}
			ReleaseDC(hwnd_, hdc_);
		}
	}
#elif defined(__ANDROID__)
#endif
	
	return 0;
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
HDC WinRcViewer::hdc() {
	return hdc_;
}

HGLRC WinRcViewer::hrc() {
	return hrc_;
}
#elif defined(__ANDROID__)
#endif
