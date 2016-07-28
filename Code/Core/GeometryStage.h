#pragma once
#include "MathAndGeometry.h"
#include "Clipper.h"
#include "Profiler.h"


SHAKURAS_BEGIN;


template<class UL, class V, class VS>
class GeometryStage {
public:
	void initialize(float w, float h, Profiler& profiler) {
		width_ = w;
		height_ = h;
		profiler_ = &profiler;
	}

	void process(DrawCall<UL, V>& call) {
		profiler_->geo_triangle_count_ += call.prims.tris_.size();

		VS vertshader;

		//vertex sharding
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			vertshader.process(call.uniforms, *i);
			profiler_->vert_sharder_excuted_++;
		}

		//geometry sharding£¬Î´ÊµÏÖ

		//projection transform
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			i->pos = call.proj_trsf.transform(i->pos);
		}

		//cliping
		Clipper<V> clipper(call.prims);
		clipper.process();

		//screen mapping
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			screenMapping(i->pos);
		}
	}

private:
	void screenMapping(Vector4f& v) {
		float w = v.w;
		float rhw = 1.0f / w;

		Vector4f r;
		r.x = (v.x * rhw + 1.0f) * width_ * 0.5f;
		r.y = (1.0f - v.y * rhw) * height_ * 0.5f;
		r.z = v.z * rhw;
		r.w = w;

		v = r;
	}


private:
	float width_, height_;
	Profiler* profiler_;
};


SHAKURAS_END;