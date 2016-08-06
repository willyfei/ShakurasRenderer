#pragma once
#include "GlBatch.h"
#include "GlUniformList.h"


SHAKURAS_BEGIN;


struct GlDrawCall {
	GlUniformList uniforms;
	std::vector<GlBatchPtr> batchs;
};


SHAKURAS_END;