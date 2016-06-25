#pragma once
#include "UtilityAndMath.h"


SHAKURAS_BEGIN;


class GsTransformer {
public:
	GsTransformer() {}
	
public:
	void initialize(float width, float height) {
		float aspect = width / height;
		world.reset();
		view.reset();
		projection = Matrix44f::Perspective(kGSPI * 0.5f, aspect, 1.0f, 500.0f);
		w = width;
		h = height;
		updateWVP();
	}

	void updateWVP() {
		wvp = (world * view) * projection;
	}

	Vector4f homogenize(const Vector4f& v) const {
		Vector4f r;
		float rhw = 1.0f / v.w;
		r.x = (v.x * rhw + 1.0f) * w * 0.5f;
		r.y = (1.0f - v.y * rhw) * h * 0.5f;
		r.z = v.z * rhw;
		r.w = 1.0f;
		return r;
	}

public:
	Matrix44f world;
	Matrix44f view;
	Matrix44f projection;
	Matrix44f wvp;
	float w, h;
};


SHAKURAS_SHARED_PTR(GsTransformer);


SHAKURAS_END;