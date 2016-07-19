#pragma once
#include "MathAndGeometry.h"
#include "VaryingList.h"
#include <array>


SHAKURAS_BEGIN;


enum VertexCategory {
	kVCVertex = 0,
	kVCTriangle = 1
};


template<class AttribList, class VaryingList>
class Vertex {
public:
	Vertex() : rhw(1.0f), cat(kVCVertex) {}
	Vertex(const Vector4f& ppos)
		: pos(ppos), cat(kVCVertex) {}
	Vertex(const Vector4f& ppos, const VaryingList& vvar, float rrhw, short ccat)
		: pos(ppos), varyings(vvar), rhw(rrhw), cat(ccat) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		PerspectMul(varyings, rhw);
	}

public:
	AttribList attribs;
	VaryingList varyings;
	Vector4f pos;
	float rhw;
	short cat;
};


template<class A, class V>
void PerspectMul(Vertex<A, V>& v1, float t) {
	v1.pos = v1.pos * t;
	PerspectMul(v1.varyings, t);
	v1.rhw = v1.rhw * t
}

template<class A, class V>
void LerpMul(Vertex<A, V>& v1, float t) {
	v1.pos = v1.pos * t;
	LerpMul(v1.varyings, t);
	v1.rhw = v1.rhw * t;
}

template<class A, class V>
void LerpPlus(Vertex<A, V>& v1, const Vertex<A, V>& v2) {
	v1.pos = v1.pos + v2.pos;
	LerpPlus(v1.varyings, v2.varyings);
	v1.rhw = v1.rhw + v2.rhw;
}

template<class A, class V>
void LerpSub(Vertex<A, V>& v1, const Vertex<A, V>& v2) {
	v1.pos = v1.pos - v2.pos;
	LerpSub(v1.varyings, v2.varyings);
	v1.rhw = v1.rhw - v2.rhw;
}


SHAKURAS_END;