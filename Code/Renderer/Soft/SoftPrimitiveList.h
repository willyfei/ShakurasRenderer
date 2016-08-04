#pragma once
#include "../Utility.h"
#include <assert.h>


SHAKURAS_BEGIN;


template<class V> 
class SoftPrimitiveList {
public:
	void clear() {
		verts_.clear();
		tris_.clear();
	}

	void swap(SoftPrimitiveList<V>& src) {
		std::swap(verts_, src.verts_);
		std::swap(tris_, src.tris_);
	}

	void merge(const SoftPrimitiveList<V>& src) {
		verts_.reserve(verts_.size() + src.verts_.size());
		tris_.reserve(tris_.size() + src.tris_.size());

		size_t len = verts_.size();
		verts_.insert(verts_.end(), src.verts_.begin(), src.verts_.end());
		for (auto i = src.tris_.begin(); i != src.tris_.end(); i++) {
			tris_.push_back({ (*i)[0] + len, (*i)[1] + len, (*i)[2] + len });
		}
	}

	void addTriangle(const V& v1, const V& v2, const V& v3) {
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