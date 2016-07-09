#pragma once
#include "MathAndGeometry.h"
#include <array>


SHAKURAS_BEGIN;


template<class Attrib, class Varying>
class GsVertex {
public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos)
		: pos(ppos) {}
	GsVertex(const Vector4f& ppos, const Varying& vvar, float rrhw)
		: pos(ppos), varying(vvar), rhw(rrhw) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		varying = varying * rhw;
	}

public: 
	Vector4f pos;
	float rhw;
	Attrib attrib;
	Varying varying;
};


template<class A, class V>
GsVertex<A, V> operator+(const GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	return GsVertex<A, V>(v1.pos + v2.pos, v1.varying + v2.varying, v1.rhw + v2.rhw);
}
template<class A, class V>
GsVertex<A, V> operator-(const GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	return GsVertex<A, V>(v1.pos - v2.pos, v1.varying - v2.varying, v1.rhw - v2.rhw);
}
template<class A, class V>
GsVertex<A, V> operator*(const GsVertex<A, V>& v1, float t) {
	return GsVertex<A, V>(v1.pos * t, v1.varying * t, v1.rhw * t);
}
template<class A, class V>
GsVertex<A, V> operator/(const GsVertex<A, V>& v1, float d) {
	float t = 1.0f / d;
	return GsVertex<A, V>(v1.pos * t, v1.varying * t, v1.rhw * t);
}


template<class Varying>
class GsFragment {
public:
	GsFragment() : x(0), y(0), z(1.0f) {}
	GsFragment(int xx, int yy, const Varying& vvar, float zz)
		: x(xx), y(yy), varying(vvar), z(zz) {}

public:
	int x, y;
	float z;
	Varying varying;
	Vector4f c;
};


SHAKURAS_END;