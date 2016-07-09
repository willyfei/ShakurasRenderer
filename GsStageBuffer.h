#pragma once
#include "Utility.h"
#include "GsViewer.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


template<class Uniform, class Vert>
struct GsStageBuffer {
	Uniform uniform;
	std::vector<Vert> vertlist;
	std::vector<int> itris;
};


SHAKURAS_END;