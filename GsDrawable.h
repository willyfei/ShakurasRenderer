#pragma once
#include "GsState.h"
#include "GsPrimitive.h"


SHAKURAS_BEGIN;


struct GsDrawable {
	Matrix44f modeltrsf;
	GsStatePtr state;
	std::vector<GsTriangle> tris;
};


SHAKURAS_SHARED_PTR(GsDrawable);


SHAKURAS_END;