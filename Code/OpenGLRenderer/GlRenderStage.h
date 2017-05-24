#pragma once
#include "GlRendererDll.h"
#include "GlDrawCall.h"
#include "Core\Profiler.h"

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
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
		setContext(viewer->hdc(), viewer->hrc());
#elif defined(__ANDROID__)
		setContext(app_);
#endif
		initGl();
	}

	void clean();

	void process(std::vector<GlDrawCall>& calls);

private:
	void initGl();
	Profiler* profiler_;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	void setContext(HDC hdc, HGLRC hrc);
	HGLRC hrc_;
	HDC hdc_;

#elif defined(__ANDROID__)
	void setContext(android_app* app);
	struct android_app* app_;
#endif

};


SHAKURAS_END;

#pragma warning(pop) 