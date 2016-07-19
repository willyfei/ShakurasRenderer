#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


enum UserMessage {
	kUMUp = 0,
	kUMDown,
	kUMLeft,
	kUMRight,
	kUMEsc,
	kUMSpace,
	kUMClose
};


class Viewer {
public:
	virtual ~Viewer() {}
	virtual int initialize(int w, int h, const char* title) = 0;
	virtual bool testUserMessage(UserMessage msg) = 0;
	virtual void dispatch() = 0;
	virtual void update(void) = 0;
	virtual void* frameBuffer() = 0;
	virtual int width() = 0;
	virtual int height() = 0;
};


SHAKURAS_SHARED_PTR(Viewer);


//Windows
ViewerPtr CreateGsViewer(std::string platform);


SHAKURAS_END;