#include "stdafx.h"
#include "GsGeometryStage.h"


SHAKURAS_BEGIN;


void GsGeometryStage::initialize(float w, float h) {
	width_ = w;
	height_ = h;
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


inline void ScreenMapping(Vector4f& v, float width, float height) {
	float w = v.w;
	Vector4f r;
	float rhw = 1.0f / v.w;
	r.x = (v.x * rhw + 1.0f) * width * 0.5f;
	r.y = (1.0f - v.y * rhw) * height * 0.5f;
	r.z = v.z * rhw;
	r.w = w;
	v = r;
}


inline void Transform(const Matrix44f& m, GsVertexA4& v) {
	v.pos = m.transform(v.pos);
}


void GsGeometryStage::process(GsGeometryStage::In& input, GsGeometryStage::Out& output) {
	for (int i = 0; i != input.itris.size(); i++) {
		//model view transform
		Matrix44f mvtrsf = input.modeltrsflist[input.itrsfs[i]] * input.viewtrsf;

		Transform(mvtrsf, input.vertlist[input.itris[i][0]]);
		Transform(mvtrsf, input.vertlist[input.itris[i][1]]);
		Transform(mvtrsf, input.vertlist[input.itris[i][2]]);

		//vertex sharding, 未实现

		//projection transform
		Transform(input.projtrsf, input.vertlist[input.itris[i][0]]);
		Transform(input.projtrsf, input.vertlist[input.itris[i][1]]);
		Transform(input.projtrsf, input.vertlist[input.itris[i][2]]);
	}


	//cliping, 简化实现
	size_t i = 0;
	while (i < input.itris.size()) {
		if (CheckCVV(input.vertlist[input.itris[i][0]].pos) != 0 ||
			CheckCVV(input.vertlist[input.itris[i][1]].pos) != 0 ||
			CheckCVV(input.vertlist[input.itris[i][2]].pos) != 0) {
			std::swap(input.itris[i], input.itris.back());
			std::swap(input.itrsfs[i], input.itrsfs.back());
			input.itris.pop_back();
			input.itrsfs.pop_back();
		}
		else {
			i++;
		}
	}

	//screen mapping
	for (auto i = input.vertlist.begin(); i != input.vertlist.end(); i++) {
		ScreenMapping(i->pos, width_, height_);
	}

	//output
	std::swap(input.vertlist, output.vertlist);
	std::swap(input.texture, output.texture);
	std::swap(input.itris, output.itris);
}


SHAKURAS_END;