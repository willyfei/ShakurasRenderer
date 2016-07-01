#include "stdafx.h"
#include "GsGeometryStage.h"


SHAKURAS_BEGIN;


void GsGeometryStage::initialize(float w, float h) {
	width_ = w;
	height_ = h;
	Vector4f eye = { 3, 0, 0, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	viewtrsf_ = Matrix44f::LookAt(eye, at, up);
	projtrsf_ = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 1.0f, 500.0f);
}


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


template<class It>
bool CheckCVV(It b, It e) {
	for (; b != e; b++) {
		if (CheckCVV(b->pos) != 0) {
			return false;
		}
	}
	return true;
}


template<class It>
bool Transform(It b, It e, const Matrix44f& m) {
	for (; b != e; b++) {
		b->pos = m.transform(b->pos);
	}
	return true;
}


inline Vector4f ScreenMapping(const Vector4f& v, float width, float height) {
	Vector4f r;
	float rhw = 1.0f / v.w;
	r.x = (v.x * rhw + 1.0f) * width * 0.5f;
	r.y = (1.0f - v.y * rhw) * height * 0.5f;
	r.z = v.z * rhw;
	r.w = 1.0f;
	return r;
}

template<class It>
bool ScreenMapping(It b, It e, float width, float height) {
	for (; b != e; b++) {
		float w = b->pos.w;
		b->pos = ScreenMapping(b->pos, width, height);
		b->pos.w = w;
	}
	return true;
}


void GsGeometryStage::process(GsGeometryStage::In& input, GsGeometryStage::Out& output) {
	viewtrsf_ = input.viewtrsf;

	for (auto i = input.objects.begin(); i != input.objects.end(); i++) {
		IGsApplicationStage::Object& object = *i;

		//model view transform
		Matrix44f mvtrsf = object.modeltrsf * viewtrsf_;

		for (size_t ii = 0; ii < object.trianglelist.size(); ii += 3) {
			GsVertex* tri = &object.trianglelist[ii];
			Transform(tri, tri + 3, mvtrsf);
		}

		//vertex sharding, Î´ÊµÏÖ

		//projection transform
		for (size_t ii = 0; ii < object.trianglelist.size(); ii += 3) {
			GsVertex* tri = &object.trianglelist[ii];
			Transform(tri, tri + 3, projtrsf_);
		}

		//cliping
		size_t ii = 0;
		while (ii < object.trianglelist.size()) {
			GsVertex* tri = &object.trianglelist[ii];
			if (!CheckCVV(tri, tri + 3)) {
				size_t len = object.trianglelist.size();
				std::swap(object.trianglelist[ii], object.trianglelist[len - 2]);
				std::swap(object.trianglelist[ii + 1], object.trianglelist[len - 1]);
				std::swap(object.trianglelist[ii + 2], object.trianglelist[len]);
				object.trianglelist.pop_back();
				object.trianglelist.pop_back();
				object.trianglelist.pop_back();
			}
			else {
				ii += 3;
			}
		}

		//screen mapping
		for (size_t ii = 0; ii < object.trianglelist.size(); ii += 3) {
			GsVertex* tri = &object.trianglelist[ii];
			ScreenMapping(tri, tri + 3, width_, height_);
		}

		//output
		Geometric geom;
		geom.texture = object.texture;
		std::swap(geom.trianglelist, object.trianglelist);

		output.push_back(geom);
	}
}


SHAKURAS_END;