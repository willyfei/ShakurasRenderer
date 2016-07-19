#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


template<class VaryingList>
class Fragment {
public:
	Fragment() : x(0), y(0), z(1.0f) {}
	Fragment(int xx, int yy, const VaryingList& vvar, float zz)
		: x(xx), y(yy), varyings(vvar), z(zz) {}

public:
	int x, y;
	float z;
	VaryingList varyings;
	Vector4f c;
};


SHAKURAS_END;