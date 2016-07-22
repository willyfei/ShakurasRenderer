#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


template<class VL>
class Fragment {
public:
	Fragment() : x(0), y(0), z(1.0f), draw(true) {}

public:
	int x, y;
	float z;
	VL varyings;
	bool draw;
	Vector4f c;
};


template<class VL>
inline Vector2f TexCoord(const VL& vl) {
	return Vector2f();
}


SHAKURAS_END;