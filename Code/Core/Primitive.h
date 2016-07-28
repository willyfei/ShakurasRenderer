#pragma once
#include "Utility.h"
#include <assert.h>


SHAKURAS_BEGIN;


template<class V> 
class PrimitiveList {
public:
	void clear() {
		verts_.clear();
		tris_.clear();
	}

	void swap(PrimitiveList<V>& src) {
		std::swap(verts_, src.verts_);
		std::swap(tris_, src.tris_);
	}

	void merge(const PrimitiveList<V>& src) {
		verts_.reserve(verts_.size() + src.verts_.size());
		tris_.reserve(tris_.size() + src.tris_.size());

		size_t len = verts_.size();
		verts_.insert(verts_.end(), src.verts_.begin(), src.verts_.end());
		for (auto i = src.tris_.begin(); i != src.tris_.end(); i++) {
			tris_.push_back({ (*i)[0] + len, (*i)[1] + len, (*i)[2] + len });
		}
	}

	void addTriangle(const V& v1, const V& v2, const V& v3) {
		if (isnan(v1.pos.x) || isnan(v1.pos.y) || isnan(v1.pos.z) || isnan(v1.pos.w) ||
			isnan(v2.pos.x) || isnan(v2.pos.y) || isnan(v2.pos.z) || isnan(v2.pos.w) ||
			isnan(v3.pos.x) || isnan(v3.pos.y) || isnan(v3.pos.z) || isnan(v3.pos.w)) {
			int fuck = 1;
		}

		if (v1.pos.w < 0.0f || v2.pos.w < 0.0f || v3.pos.w < 0.0f) {
			int fuck = 1;
		}

		size_t len = verts_.size();
		verts_.push_back(v1);
		verts_.push_back(v2);
		verts_.push_back(v3);
		tris_.push_back({ len, len + 1, len + 2 });
	}

public:
	std::vector<V> verts_;
	std::vector<std::array<size_t, 3> > tris_;//[0,1,2][3,4,5]...
};


SHAKURAS_END;