#pragma once
#include "GlBatch.h"
#include "GlProgram.h"


SHAKURAS_BEGIN;


struct GlDrawCall {
	GlAbsProgramPtr program;
	std::vector<GlBatchPtr> batchs;
};


SHAKURAS_END;