#pragma once
#include "Core/MathAndGeometry.h"
#include <Windows.h>
#include <tchar.h>
#include <map>
#include <string>


#ifdef _PLATFORMSPEC_DLL
#    define PLATFORMSPEC_DLL   __declspec(dllexport)
#else
#    define PLATFORMSPEC_DLL   __declspec(dllimport)
#endif


#pragma warning(push) 
#pragma warning(disable:4251)


enum UserMessage {
	kUMUp = 0,
	kUMDown,
	kUMLeft,
	kUMRight,
	kUMEsc,
	kUMSpace,
	kUMClose
};


class PLATFORMSPEC_DLL WinViewer : public std::enable_shared_from_this<WinViewer> {
public:
	WinViewer();
	~WinViewer() {}

public:
	int initialize(int w, int h, const char* title);

	bool testUserMessage(UserMessage msg);

	void dispatch();

	void update();

	void* frameBuffer();

	int width();

	int height();

	int close();

	LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	static std::map<HWND, std::weak_ptr<WinViewer> > objects;
};


typedef std::shared_ptr<WinViewer> WinViewerPtr;


#pragma warning(pop) 