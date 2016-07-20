#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


template<class VaryingList>
class Fragment {
public:
	Fragment() : x(0), y(0), z(1.0f), draw(true) {}

public:
	int x, y;
	float z;
	VaryingList varyings;
	bool draw;
	Vector4f c;
};


SHAKURAS_END;