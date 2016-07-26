#pragma once
#include "MathAndGeometry.h"
#include "Surface.h"
#include "Mipmap.h"


SHAKURAS_BEGIN;


class Sampler {
public:
	template<class S, typename AF>
	Vector3f surfaceNearest(float u, float v, const S& surface, AF addressing) {
		return NearestSample(u, v, surface, addressing);
	}

	template<class S, typename AF>
	Vector3f surfaceBilinear(float u, float v, const S& surface, AF addressing) {
		return BilinearSample(u, v, surface, addressing);
	}

	template<class M, typename AF>
	Vector3f mipmapNearest(float u, float v, const M& mipmap, AF addressing) {
		return NearestSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

	template<class M, typename AF>
	Vector3f mipmapBilinear(float u, float v, const M& mipmap, AF addressing) {
		return BilinearSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

	template<class M, typename AF>
	Vector3f mipmapTrilinear(float u, float v, const M& mipmap, AF addressing) {
		return TrilinearSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

public:
	Vector2f ddx_, ddy_;//uvµÄµ¼Êý
};


SHAKURAS_END;