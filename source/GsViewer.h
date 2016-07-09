#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


enum GsUserMessage {
	kUMUp = 0,
	kUMDown,
	kUMLeft,
	kUMRight,
	kUMEsc,
	kUMSpace,
	kUMClose
};


class GsViewer {
public:
	virtual ~GsViewer() {}
	virtual int initialize(int w, int h, const char* title) = 0;
	virtual bool testUserMessage(GsUserMessage msg) = 0;
	virtual void dispatch() = 0;
	virtual void update(void) = 0;
	virtual void* frameBuffer() = 0;
	virtual int width() = 0;
	virtual int height() = 0;
};


SHAKURAS_SHARED_PTR(GsViewer);


//Windows
GsViewerPtr CreateGsViewer(std::string platform);


SHAKURAS_END;