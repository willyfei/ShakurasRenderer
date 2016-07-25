#pragma once
#include "Utility.h"


SHAKURAS_BEGIN;


template<class V> 
class PrimitiveList {
public:
	std::vector<V> verts_;
	std::vector<std::array<size_t, 3> > tris_;//[0,1,2][3,4,5]...
};


SHAKURAS_END;