#pragma once
#include "SoftPrimitiveList.h"
#include "Core/MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;


template<class UL, class V>
struct SoftDrawCall {
	UL uniforms;
	Matrix44f proj_trsf;
	SoftPrimitiveList<V> prims;
};


SHAKURAS_END;