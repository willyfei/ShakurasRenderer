#pragma once
#include "Utility.h"
#include "MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;



template<class UL, class V>
struct StageBuffer {
	UL uniforms;
	Matrix44f projtrsf;
	std::vector<V> vertlist;
};


SHAKURAS_END;