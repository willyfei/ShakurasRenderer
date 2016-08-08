#include "GlRenderStage.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"


SHAKURAS_BEGIN;


class GlContextBinding {
public:
	GlContextBinding(HDC hDC, HGLRC hRC) {
		wglMakeCurrent(hDC, hRC);
	}

	~GlContextBinding() {
		wglMakeCurrent(NULL, NULL);
	}
};


GlRenderStage::GlRenderStage() {
	profiler_ = nullptr;
	hrc_ = nullptr;
	hdc_ = nullptr;
}


void GlRenderStage::clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void GlRenderStage::initContext(HDC hdc) {
	hrc_ = wglCreateContext(hdc);
	hdc_ = hdc;
	wglMakeCurrent(hdc_, hrc_);
}


void GlRenderStage::initStaticState() {
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
		i->program->prepare();
		std::for_each(i->batchs.begin(), i->batchs.end(), draw_batch);
	}
}


SHAKURAS_END;