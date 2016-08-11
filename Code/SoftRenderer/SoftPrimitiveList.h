#pragma once
#include "SoftVertex.h"
#include <assert.h>


SHAKURAS_BEGIN;


template<class A, class V> 
class SoftPrimitiveList {
public:
	void clear() {
		verts_.clear();
		tris_.clear();
	}

public:
	std::vector<SoftVertex<A, V> > verts_;
	std::vector<std::array<size_t, 3> > tris_;//[0,1,2][3,4,5]...
};


SHAKURAS_END;