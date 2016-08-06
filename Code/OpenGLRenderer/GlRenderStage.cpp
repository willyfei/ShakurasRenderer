#include "GlRenderStage.h"
#include "gl\glew.h"


SHAKURAS_BEGIN;


class GlContextBinding {
public:
	GlContextBinding(HDC hDC, HGLRC hRC) {
		hDC_ = hDC;
		m_hHC = hRC;
		wglMakeCurrent(hDC_, m_hHC);
	}

	~GlContextBinding() {
		wglMakeCurrent(NULL, NULL);
	}

private:
	HDC hDC_;
	HGLRC m_hHC;
};


GlRenderStage::GlRenderStage() {
	profiler_ = nullptr;
	hrc_ = nullptr;
	hdc_ = nullptr;
}


void GlRenderStage::clean() {
	GlContextBinding ctx(hdc_, hrc_);

	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void GlRenderStage::initContext(HDC hdc) {
	hrc_ = wglCreateContext(hdc);
	hdc_ = hdc;
}


void GlRenderStage::initStaticState() {
	GlContextBinding ctx(hdc_, hrc_);

	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.1, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D);

	//透明材质
	glAlphaFunc(GL_GREATER, 0.3f);//0.5可以换成任何在0~1之间的数 
	glPolygonMode(GL_FRONT, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
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
		std::for_each(i->batchs.begin(), i->batchs.end(), draw_batch);
	}
}


SHAKURAS_END;