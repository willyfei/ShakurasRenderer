#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


class Sampler {
public:
	template<class S>
	Vector3f surfaceNearest(float u, float v, const S& surface) {
		return NearestSample(u, v, surface);
	}

	template<class S>
	Vector3f surfaceBilinear(float u, float v, const S& surface) {
		return BilinearSample(u, v, surface);
	}

public:
	Vector2f ddx_, ddy_;
};


SHAKURAS_END;