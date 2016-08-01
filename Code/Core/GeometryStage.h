#pragma once
#include "MathAndGeometry.h"
#include "Clipper.h"
#include "Profiler.h"
#include <ppl.h>


SHAKURAS_BEGIN;


template<class UL, class V, class VS>
class GeometryStage {
public:
	void initialize(float w, float h, Profiler& profiler) {
		width_ = w;
		height_ = h;
		profiler_ = &profiler;
		refuse_back_ = true;
	}

	void process(DrawCall<UL, V>& call) {
		profiler_->count("Geo-Triangle Count", (int)call.prims.tris_.size());

		//vertex sharding
		auto vert_sharding = [&](V& vert) {
			VS().process(call.uniforms, vert);
		};

		profiler_->count("Vert-Sharder Excuted", (int)call.prims.verts_.size());
		Concurrency::parallel_for_each(call.prims.verts_.begin(), call.prims.verts_.end(), vert_sharding);

		//geometry sharding£¬Î´ÊµÏÖ

		//projection transform
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			i->pos = call.proj_trsf.transform(i->pos);
		}

		//cliping
		Clipper<V>(call.prims, *profiler_, refuse_back_).process();

		//screen mapping
		for (auto i = call.prims.verts_.begin(); i != call.prims.verts_.end(); i++) {
			screenMapping(i->pos);
		}
	}

	void refuseBack(bool rb) {
		refuse_back_ = rb;
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
	bool refuse_back_;
	Profiler* profiler_;
};


SHAKURAS_END;