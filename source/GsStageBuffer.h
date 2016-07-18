#pragma once
#include "Utility.h"
#include "MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;


enum GsVertexCategory {
	kVCVertex = 0,
	kVCTriangleBegin = 1
};


template<class UniformList, class Vert>
struct GsStageBuffer {
	UniformList uniforms;
	Matrix44f projtrsf;
	std::vector<Vert> vertlist;
	std::vector<short> vclist;
};


SHAKURAS_END;