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

GLuint g_vao = 0;
GLuint g_ele = 0;
GlColorProgram g_prag;

void InitTest() {
	return;

	GLuint vbo = 0;
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	GLfloat vertices[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f };

	glGenBuffers(1, &g_ele);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ele);
	uint16_t array_ele[] = { 0, 1, 2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3, array_ele, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	g_prag.initialize();
	g_prag.use();
	g_prag.setColor({ 1.0f, 0.0f, 1.0f });
}


void DrawTest() {
	return;

	g_prag.use();

	glBindVertexArray(g_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ele);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);

	/*glBindVertexArray(g_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);*/
}


void GlRenderStage::initGl() {
	GlContextBinding ctx(hdc_, hrc_);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);

	InitTest();
}


void GlRenderStage::process(std::vector<GlDrawCall>& calls) {
	GlContextBinding ctx(hdc_, hrc_);

	clean();
	
	DrawTest();

	auto draw_batch = [&](GlBatchPtr batch) {
		if (batch) {
			batch->draw();
		}
	};

	for (auto i = calls.begin(); i != calls.end(); i++) {
		i->program->use();
		std::for_each(i->batchs.begin(), i->batchs.end(), draw_batch);
	}

	glFinish();
}


void GlRenderStage::setContext(HDC hdc, HGLRC hrc) {
	hdc_ = hdc;
	hrc_ = hrc;
}


SHAKURAS_END;