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

	template<class M>
	Vector3f mipmapNearest(float u, float v, const M& mipmap) {
		return NearestSample(u, v, ddx_, ddy_, mipmap);
	}

	template<class M>
	Vector3f mipmapBilinear(float u, float v, const M& mipmap) {
		return BilinearSample(u, v, ddx_, ddy_, mipmap);
	}

	template<class M>
	Vector3f mipmapTrilinear(float u, float v, const M& mipmap) {
		return TrilinearSample(u, v, ddx_, ddy_, mipmap);
	}

public:
	Vector2f ddx_, ddy_;//uvµÄµ¼Êý
};


SHAKURAS_END;