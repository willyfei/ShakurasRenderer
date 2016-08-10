#pragma once
#include "GlRendererDll.h"
#include "GlDrawCall.h"
#include "Core\Profiler.h"
#include <Windows.h>

#pragma warning(push) 
#pragma warning(disable:4251)


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlRenderStage {
public:
	GlRenderStage();

public:
	template<class VPTR>
	void initialize(VPTR viewer, Profiler& profiler) {
		profiler_ = &profiler;
		setContext(viewer->hdc(), viewer->hrc());
		initGl();
	}

	void clean();

	void process(std::vector<GlDrawCall>& calls);

private:
	void initGl();

	void setContext(HDC hdc, HGLRC hrc);

private:
	Profiler* profiler_;
	HGLRC hrc_;
	HDC hdc_;
};


SHAKURAS_END;

#pragma warning(pop) 