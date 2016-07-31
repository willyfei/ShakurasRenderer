#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


template<class VL>
class Fragment {
public:
	Fragment() : x(0), y(0), z(1.0f), weight(0.0f) {}

public:
	int x, y;
	float z;
	VL varyings;
	float weight;
	Vector4f c;
};


template<class VL>
inline Vector2f TexCoord(const VL& vl) {
	return Vector2f();
}


SHAKURAS_END;