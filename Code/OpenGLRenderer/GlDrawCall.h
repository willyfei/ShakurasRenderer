#pragma once
#include "GlRendererDll.h"
#include "GlBatch.h"
#include "GlProgram.h"


#pragma warning(push)
#pragma warning(disable:4251)

SHAKURAS_BEGIN;


struct GLRENDERER_DLL GlDrawCall {
	GlAbsProgramPtr program;
	std::vector<GlBatchPtr> batchs;
};


SHAKURAS_END;

#pragma warning(pop)