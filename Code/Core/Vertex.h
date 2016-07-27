#pragma once
#include "MathAndGeometry.h"
#include <array>


SHAKURAS_BEGIN;


template<class A, class V>
class Vertex {
public:
	Vertex() : rhw(1.0f) {}
	Vertex(const Vector4f& ppos)
		: pos(ppos) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		varyings = varyings * rhw;
	}

public:
	Vector4f pos;
	float rhw;
	A attribs;
	V varyings;
};


template<class A, class V>
inline Vertex<A, V> operator+(const Vertex<A, V>& v1, const Vertex<A, V>& v2) {
	Vertex<A, V> v3;
	v3.pos = v1.pos + v2.pos;
	v3.varyings = v1.varyings + v2.varyings;
	v3.rhw = v1.rhw + v2.rhw;
	return v3;
}

template<class A, class V>
inline Vertex<A, V> operator-(const Vertex<A, V>& v1, const Vertex<A, V>& v2) {
	Vertex<A, V> v3;
	v3.pos = v1.pos - v2.pos;
	v3.varyings = v1.varyings - v2.varyings;
	v3.rhw = v1.rhw - v2.rhw;
	return v3;
}

template<class A, class V>
inline Vertex<A, V> operator*(const Vertex<A, V>& v1, float t) {
	Vertex<A, V> v3;
	v3.pos = v1.pos * t;
	v3.varyings = v1.varyings * t;
	v3.rhw = v1.rhw * t;
	return v3;
}

template<class A, class V>
inline Vertex<A, V> operator/(const Vertex<A, V>& v1, float d) {
	Vertex<A, V> v3;
	float t = 1.0f / d;
	v3.pos = v1.pos * t;
	v3.varyings = v1.varyings * t;
	v3.rhw = v1.rhw * t;
	return v3;
}


SHAKURAS_END;