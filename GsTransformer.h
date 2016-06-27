#pragma once
#include "MathAndGeometry.h"
#include "GsPrimitive.h"


SHAKURAS_BEGIN;


class GsTransformer {
public:
	GsTransformer() {}
	
public:
	void initialize(float w, float h);

	void updateWVP();

	Vector4f homogenize(const Vector4f& v) const;

public:
	Matrix44f world;
	Matrix44f view;
	Matrix44f projection;
	Matrix44f wvp;
	float width, height;
};


SHAKURAS_SHARED_PTR(GsTransformer);


inline int CheckCVV(const Vector4f& v) {
	int check = 0;
	if (v.z < 0.0f) check |= 1;
	if (v.z > v.w) check |= 2;
	if (v.x < -v.w) check |= 4;
	if (v.x > v.w) check |= 8;
	if (v.y < -v.w) check |= 16;
	if (v.y > v.w) check |= 32;
	return check;
}


void CameraAtZero(GsTransformerPtr& trsf, float x, float y, float z);


SHAKURAS_END;