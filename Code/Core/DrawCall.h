#pragma once
#include "Utility.h"
#include "MathAndGeometry.h"
#include "Primitive.h"
#include <vector>


SHAKURAS_BEGIN;


template<class UL, class V>
struct DrawCall {
	UL uniforms;
	Matrix44f projtrsf;
	PrimitiveList<V> prims;
};


SHAKURAS_END;