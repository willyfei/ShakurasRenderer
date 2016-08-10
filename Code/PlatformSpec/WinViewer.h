#pragma once
#include "Core/MathAndGeometry.h"
#include <Windows.h>
#include <tchar.h>
#include <map>


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
	virtual ~WinViewer() {}

public:
	virtual int initialize(int w, int h, const char* title) = 0;

	virtual bool testUserMessage(UserMessage msg);

	virtual void dispatch();

	virtual void update() = 0;

	virtual int width();

	virtual int height();

	virtual int close() = 0;

	virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	int width_, height_;
	std::array<int, 512> keys_;
	bool close_;
	HWND hwnd_;
};


typedef std::shared_ptr<WinViewer> WinViewerPtr;


class PLATFORMSPEC_DLL WinMemViewer : public WinViewer {
public:
	WinMemViewer();
	virtual ~WinMemViewer() {}

public:
	virtual int initialize(int w, int h, const char* title);

	virtual void update();

	virtual int close();

	void* frameBuffer();

	HDC hdc() { return NULL; }
	HGLRC hrc() { return NULL; }

protected:
	HDC hdc_;
	HBITMAP hbmp_;
	HBITMAP org_hbmp_;
	unsigned char* frame_buffer_;
};


typedef std::shared_ptr<WinMemViewer> WinMemViewerPtr;


class PLATFORMSPEC_DLL WinRcViewer : public WinViewer {
public:
	WinRcViewer();
	virtual ~WinRcViewer() {}

public:
	virtual int initialize(int w, int h, const char* title);

	virtual void update();

	virtual int close();

	HDC hdc();

	HGLRC hrc();

protected:
	HDC hdc_;
	HGLRC hrc_;
};


typedef std::shared_ptr<WinRcViewer> WinRcViewerPtr;


#pragma warning(pop) 