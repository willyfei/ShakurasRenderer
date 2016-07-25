#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


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


template<class UL, class V, class VS>
class GeometryStage {
public:
	void initialize(float w, float h) {
		width_ = w;
		height_ = h;
	}

	void process(DrawCall<UL, V>& call) {
		VS vertshader;

		//vertex sharding
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			vertshader.process(call.uniforms, *i);
		}

		//geometry sharding，未实现


		//projection transform
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			i->pos = call.projtrsf.transform(i->pos);
		}

		//cliping, 简化实现
		for (size_t i = 0; i != call.prims.tris_.size();) {
			const std::array<size_t, 3>& tri = call.prims.tris_[i];

			const V& v1 = call.prims.verts_[tri[0]];
			const V& v2 = call.prims.verts_[tri[1]];
			const V& v3 = call.prims.verts_[tri[2]];

			if (CheckCVV(v1.pos) != 0 ||
				CheckCVV(v2.pos) != 0 ||
				CheckCVV(v3.pos) != 0) {
				UnstableErase(call.prims.tris_, i, 1);
			}
			else {
				i++;
			}
		}

		//screen mapping
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			ScreenMapping(i->pos, width_, height_);
		}
	}

private:
	float width_, height_;
};


SHAKURAS_END;