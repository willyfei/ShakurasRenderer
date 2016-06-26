#pragma once
#include "UtilityAndMath.h"


SHAKURAS_BEGIN;


class GsTransformer {
public:
	GsTransformer() {}
	
public:
	void initialize(float w, float h) {
		float aspect = w / h;
		world.reset();
		view.reset();
		projection = Matrix44f::Perspective(kGSPI * 0.5f, aspect, 1.0f, 500.0f);
		width = w;
		height = h;
		updateWVP();
	}

	void updateWVP() {
		wvp = (world * view) * projection;
	}

	Vector4f homogenize(const Vector4f& v) const {
		Vector4f r;
		float rhw = 1.0f / v.w;
		r.x = (v.x * rhw + 1.0f) * width * 0.5f;
		r.y = (1.0f - v.y * rhw) * height * 0.5f;
		r.z = v.z * rhw;
		r.w = 1.0f;
		return r;
	}

public:
	Matrix44f world;
	Matrix44f view;
	Matrix44f projection;
	Matrix44f wvp;
	float width, height;
};


SHAKURAS_SHARED_PTR(GsTransformer);


SHAKURAS_END;