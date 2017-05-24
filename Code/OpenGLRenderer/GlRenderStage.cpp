#include "GlRenderStage.h"
#include "GlContextBinding.h"
#include "GlPreset.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif


SHAKURAS_BEGIN;


GlRenderStage::GlRenderStage() {
	profiler_ = nullptr;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	hrc_ = nullptr;
	hdc_ = nullptr;
#elif defined(__ANDROID__)
	app_ = nullptr;
#endif
	
}


void GlRenderStage::clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void GlRenderStage::initGl() {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	GlContextBinding ctx(hdc_, hrc_);
#elif defined(__ANDROID__)
	GlContextBinding ctx(app_);
#endif

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
}


void GlRenderStage::process(std::vector<GlDrawCall>& calls) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	GlContextBinding ctx(hdc_, hrc_);
#elif defined(__ANDROID__)
	GlContextBinding ctx(app_);
#endif

	clean();
	
	auto draw_batch = [&](GlBatchPtr batch) {
		if (batch) {
			batch->draw();
		}
	};

	for (auto i = calls.begin(); i != calls.end(); i++) {
		GlAbsProgramPtr prog = i->program;
		if (!prog) {
			continue;
		}
		prog->use();
		prog->prepare();
		std::for_each(i->batchs.begin(), i->batchs.end(), draw_batch);
	}

	glFinish();
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
void GlRenderStage::setContext(HDC hdc, HGLRC hrc) {
	hdc_ = hdc;
	hrc_ = hrc;
}
#elif defined(__ANDROID__)
void GlRenderStage::setContext(android_app* app) {
	app_ = app;
}
#endif



SHAKURAS_END;