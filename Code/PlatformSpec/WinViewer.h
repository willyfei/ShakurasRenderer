#pragma once
#include "Core/MathAndGeometry.h"
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <Windows.h>
#include <tchar.h>
#elif defined(__ANDROID__)
#endif
#include <map>
#include <array>

#ifdef _PLATFORMSPEC_DLL
#    define PLATFORMSPEC_DLL   KLAYGE_SYMBOL_EXPORT
#else
#    define PLATFORMSPEC_DLL   KLAYGE_SYMBOL_IMPORT
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


protected:
	int width_, height_;
	std::array<int, 512> keys_;
	bool close_;
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	HWND hwnd_;
public:
	virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#elif defined(__ANDROID__)
#endif
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

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	HDC hdc() { return NULL; }
	HGLRC hrc() { return NULL; }

protected:
	HDC hdc_;
	HBITMAP hbmp_;
	HBITMAP org_hbmp_;
#elif defined(__ANDROID__)
#endif

protected:
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

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	HDC hdc();

	HGLRC hrc();

protected:
	HDC hdc_;
	HGLRC hrc_;
#elif defined(__ANDROID__)
#endif

};


typedef std::shared_ptr<WinRcViewer> WinRcViewerPtr;


#pragma warning(pop) 