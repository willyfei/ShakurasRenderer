#pragma once
#include "MathAndGeometry.h"
#include "Clipper.h"
#include "Profiler.h"


SHAKURAS_BEGIN;


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
	void initialize(float w, float h, Profiler& profiler) {
		width_ = w;
		height_ = h;
		profiler_ = &profiler;
	}

	void process(DrawCall<UL, V>& call) {
		VS vertshader;

		//vertex sharding
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			vertshader.process(call.uniforms, *i);
			profiler_->vert_sharder_excuted_++;
		}

		//geometry sharding£¬Î´ÊµÏÖ


		//projection transform
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			i->pos = call.projtrsf.transform(i->pos);
		}

		//cliping
		Clipper<V> clipper(call.prims);
		clipper.process();

		//screen mapping
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			ScreenMapping(i->pos, width_, height_);
		}
	}

private:
	float width_, height_;
	Profiler* profiler_;
};


SHAKURAS_END;