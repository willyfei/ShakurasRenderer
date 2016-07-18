#pragma once
#include "Utility.h"
#include "MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;



template<class UniformList, class Vert>
struct GsStageBuffer {
	UniformList uniforms;
	Matrix44f projtrsf;
	std::vector<Vert> vertlist;
};


SHAKURAS_END;