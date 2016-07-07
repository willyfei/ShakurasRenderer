#pragma once
#include "MathAndGeometry.h"
#include <array>


SHAKURAS_BEGIN;


template<int N> class GsPtrArray : public std::array<void*, N> {};


template<int N> class GsInterpArray : public std::array<float, N> {};


template<int N>
GsInterpArray<N> operator+(const GsInterpArray<N>& a1, const GsInterpArray<N>& a2) {
	GsInterpArray<N> res;
	for (int i = 0; i != N; i++) {
		res[i] = a1[i] + a2[i];
	}
	return res;
}
template<int N>
GsInterpArray<N> operator-(const GsInterpArray<N>& a1, const GsInterpArray<N>& a2) {
	GsInterpArray<N> res;
	for (int i = 0; i != N; i++) {
		res[i] = a1[i] - a2[i];
	}
	return res;
}
template<int N>
GsInterpArray<N> operator*(const GsInterpArray<N>& a1, float t) {
	GsInterpArray<N> res;
	for (int i = 0; i != N; i++) {
		res[i] = a1[i] * t;
	}
	return res;
}
template<int N>
GsInterpArray<N> operator/(const GsInterpArray<N>& v1, float d) {
	float t = 1.0f / d;
	GsInterpArray<N> res;
	for (int i = 0; i != N; i++) {
		res[i] = a1[i] * t;
	}
	return res;
}


template<int A, int V>
class GsVertex {
public:
	typedef GsPtrArray<A> Attrib;
	typedef GsInterpArray<V> Varying;

public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos)
		: pos(ppos) {}
	GsVertex(const Vector4f& ppos, const Varying& vvar, float rrhw)
		: pos(ppos), varying(vvar), rhw(rrhw) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		for (auto i = varying.begin(); i != varying.end(); i++) {
			*i *= rhw;
		}
	}

public:
	Vector4f pos;
	float rhw;
	Attrib attrib;
	Varying varying;
};


template<int A, int V>
GsVertex<A, V> operator+(const GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	return GsVertex<A, V>(v1.pos + v2.pos, v1.varying + v2.varying, v1.rhw + v2.rhw);
}
template<int A, int V>
GsVertex<A, V> operator-(const GsVertex<A, V>& v1, const GsVertex<A, V>& v2) {
	return GsVertex<A, V>(v1.pos - v2.pos, v1.varying - v2.varying, v1.rhw - v2.rhw);
}
template<int A, int V>
GsVertex<A, V> operator*(const GsVertex<A, V>& v1, float t) {
	return GsVertex<A, V>(v1.pos * t, v1.varying * t, v1.rhw * t);
}
template<int A, int V>
GsVertex<A, V> operator/(const GsVertex<A, V>& v1, float d) {
	float t = 1.0f / d;
	return GsVertex<A, V>(v1.pos * t, v1.varying * t, v1.rhw * t);
}


template<int V>
class GsFragment {
public:
	typedef GsInterpArray<V> Varying;

public:
	GsFragment() : x(0), y(0), z(1.0f), c(0) {}
	GsFragment(int xx, int yy, const Varying& vvar, float zz)
		: x(xx), y(yy), varying(vvar), z(zz) {}

public:
	int x, y;
	float z;
	Varying varying;
	uint32_t c;
};


typedef GsVertex<8, 4> GsVertexA8V4;
typedef GsFragment<4> GsFragmentV4;


SHAKURAS_END;