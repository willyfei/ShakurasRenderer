#pragma once
#include "GlDrawCall.h"
#include "Core\Profiler.h"
#include <Windows.h>


SHAKURAS_BEGIN;


class GlRenderStage {
public:
	GlRenderStage();

public:
	template<class VPTR>
	void initialize(VPTR viewer, Profiler& profiler) {
		profiler_ = &profiler;
		initContext(viewer->hdc());
		initStaticState();
		initProjection(viewer->width(), viewer->height());
	}

	void clean();

	void process(GlDrawCall& call) {

	}

private:
	void initContext(HDC hdc);
	void initStaticState();
	void initProjection(int width, int height);

private:
	Profiler* profiler_;
	HGLRC hrc_;
	HDC hdc_;
};


SHAKURAS_END;