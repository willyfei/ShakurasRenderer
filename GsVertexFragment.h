#pragma once
#include "MathAndGeometry.h"
#include <array>

SHAKURAS_BEGIN;


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


template<class Attrib>
class GsVertex {
public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos)
		: pos(ppos) {}
	GsVertex(const Vector4f& ppos, const Attrib& aattrib)
		: pos(ppos), attrib(aattrib) {}
	GsVertex(const Vector4f& ppos, const Attrib& aattrib,  float rrhw)
		: pos(ppos), attrib(aattrib), rhw(rrhw) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		for (auto i = attrib.begin(); i != attrib.end(); i++) {
			*i *= rhw;
		}
	}

public:
	Vector4f pos;
	Attrib attrib;
	float rhw;
};


template<class Att>
GsVertex<Att> operator+(const GsVertex<Att>& v1, const GsVertex<Att>& v2) {
	return GsVertex<Att>(v1.pos + v2.pos, v1.attrib + v2.attrib, v1.rhw + v2.rhw);
}
template<class Att>
GsVertex<Att> operator-(const GsVertex<Att>& v1, const GsVertex<Att>& v2) {
	return GsVertex<Att>(v1.pos - v2.pos, v1.attrib - v2.attrib, v1.rhw - v2.rhw);
}
template<class Att>
GsVertex<Att> operator*(const GsVertex<Att>& v1, float t) {
	return GsVertex<Att>(v1.pos * t, v1.attrib * t, v1.rhw * t);
}
template<class Att>
GsVertex<Att> operator/(const GsVertex<Att>& v1, float d) {
	float t = 1.0f / d;
	return GsVertex<Att>(v1.pos * t, v1.attrib * t, v1.rhw * t);
}


template<class Varying>
class GsFragment {
public:
	GsFragment() : x(0), y(0), z(1.0f), c(0) {}
	GsFragment(int xx, int yy, const Varying& vvar, float zz)
		: x(xx), y(yy), varying(vvar), z(zz) {}

public:
	int x, y;
	Varying varying;
	float z;
	uint32_t c;
};



typedef GsVertex<GsInterpArray<4> > GsVertexA4;
typedef GsFragment<GsInterpArray<4> > GsFragmentV4;


SHAKURAS_END;