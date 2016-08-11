#include "GlRenderStage.h"
#include "GlContextBinding.h"
#include "GlPreset.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"
#include "gl\wglew.h"


SHAKURAS_BEGIN;


GlRenderStage::GlRenderStage() {
	profiler_ = nullptr;
	hrc_ = nullptr;
	hdc_ = nullptr;
}


void GlRenderStage::clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void GlRenderStage::initGl() {
	GlContextBinding ctx(hdc_, hrc_);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
}


void GlRenderStage::process(std::vector<GlDrawCall>& calls) {
	GlContextBinding ctx(hdc_, hrc_);

	clean();
	
	auto draw_batch = [&](GlBatchPtr batch) {
		if (batch) {
			batch->draw();
		}
	};

	for (auto i = calls.begin(); i != calls.end(); i++) {
		GlProgramPtr prog = i->program;
		if (!prog) {
			continue;
		}
		prog->use();
		std::for_each(i->batchs.begin(), i->batchs.end(), draw_batch);
	}

	glFinish();
}


void GlRenderStage::setContext(HDC hdc, HGLRC hrc) {
	hdc_ = hdc;
	hrc_ = hrc;
}


SHAKURAS_END;