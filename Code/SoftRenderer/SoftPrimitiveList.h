#pragma once
#include "Core/Utility.h"
#include <assert.h>


SHAKURAS_BEGIN;


template<class V> 
class SoftPrimitiveList {
public:
	void clear() {
		verts_.clear();
		tris_.clear();
	}

public:
	std::vector<V> verts_;
	std::vector<std::array<size_t, 3> > tris_;//[0,1,2][3,4,5]...
};


SHAKURAS_END;