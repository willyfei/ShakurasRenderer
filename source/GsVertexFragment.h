#pragma once
#include "MathAndGeometry.h"
#include "GsVarying.h"
#include <array>


SHAKURAS_BEGIN;


template<class AttribList, class VaryingList>
class GsVertex {
public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos)
		: pos(ppos) {}
	GsVertex(const Vector4f& ppos, const VaryingList& vvar, float rrhw)
		: pos(ppos), varying(vvar), rhw(rrhw) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		PerspectMul(varying, rhw);
	}

public: 
	Vector4f pos;
	float rhw;
	AttribList attrib;
	VaryingList varying;
};


template<class A, class V>
void PerspectMul(GsVertex<A, V>& v1, float t) {
	v1.pos = v1.pos * t;
	PerspectMul(v1.varying, t);
	v1.rhw = v1.rhw * t
}

template<class A, class V>
void LerpMul(GsVertex<A, V>& v1, float t) {
	v1.pos = v1.pos * t;
	LerpMul(v1.varying, t);
	v1.rhw = v1.rhw * t;
}

template<class A, class V>
void LerpPlus(GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	v1.pos = v1.pos + v2.pos;
	LerpPlus(v1.varying, v2.varying);
	v1.rhw = v1.rhw + v2.rhw;
}

template<class A, class V>
void LerpSub(GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	v1.pos = v1.pos - v2.pos;
	LerpSub(v1.varying, v2.varying);
	v1.rhw = v1.rhw - v2.rhw;
}


template<class VaryingList>
class GsFragment {
public:
	GsFragment() : x(0), y(0), z(1.0f) {}
	GsFragment(int xx, int yy, const VaryingList& vvar, float zz)
		: x(xx), y(yy), varying(vvar), z(zz) {}

public:
	int x, y;
	float z;
	VaryingList varying;
	Vector4f c;
};


SHAKURAS_END;