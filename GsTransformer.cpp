#include "stdafx.h"
#include "GsTransformer.h"


SHAKURAS_BEGIN;


void GsTransformer::initialize(float w, float h) {
	float aspect = w / h;
	world.reset();
	view.reset();
	projection = Matrix44f::Perspective(kGSPI * 0.5f, aspect, 1.0f, 500.0f);
	width = w;
	height = h;
	updateWVP();
}


void GsTransformer::updateWVP() {
	wvp = (world * view) * projection;
}


Vector4f GsTransformer::homogenize(const Vector4f& v) const {
	Vector4f r;
	float rhw = 1.0f / v.w;
	r.x = (v.x * rhw + 1.0f) * width * 0.5f;
	r.y = (1.0f - v.y * rhw) * height * 0.5f;
	r.z = v.z * rhw;
	r.w = 1.0f;
	return r;
}


void CameraAtZero(GsTransformerPtr& trsf, float x, float y, float z) {
	Vector4f eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	trsf->view = Matrix44f::LookAt(eye, at, up);
	trsf->updateWVP();
}


SHAKURAS_END;