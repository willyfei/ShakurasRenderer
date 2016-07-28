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

		//screen mapping
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			screenMapping(*i);
		}

		//cliping
		Clipper<V> clipper(call.prims);
		clipper.process();
	}

private:
	void screenMapping(V& v) {
		float w = v.pos.w;
		float rhw = 1.0f / w;

		V r;
		r.pos.x = (v.pos.x * rhw + 1.0f) * (width_ - 1) * 0.5f;
		r.pos.y = (1.0f - v.pos.y * rhw) * (height_ - 1) * 0.5f;
		r.pos.z = v.pos.z * rhw;
		r.pos.w = w;
		r.varyings = v.varyings *rhw;
		r.rhw = rhw;

		v = r;
	}


private:
	float width_, height_;
	Profiler* profiler_;
};


SHAKURAS_END;