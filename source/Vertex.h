#pragma once
#include "MathAndGeometry.h"
#include <array>


SHAKURAS_BEGIN;


enum PrimitiveFlag {
	kPFNil = 0,
	kPFTriangle = 1
};


enum TraversalBehavior {
	kTBNone = 0,
	kTBLerp = 1U << 0,//线性插值
	kTBPerspect = 1U << 1,//透视
	kTBAll = kTBLerp | kTBPerspect
};

//VaryList要重载这几个函数
template<class T>
void TravPlus(T& v1, const T& v2, int oper) {}

template<class T>
void TravSub(T& v1, const T& v2, int oper) {}

template<class T>
void TravMul(T& v1, float t, int oper) {}

template<class T>
void TravDiv(T& v1, float d, int oper) {}


template<class AttribList, class VaryingList>
class Vertex {
public:
	Vertex() : rhw(1.0f), primf(kPFNil) {}
	Vertex(const Vector4f& ppos)
		: pos(ppos), primf(kPFNil) {}
	Vertex(const Vector4f& ppos, const VaryingList& vvar, float rrhw, short pprimf)
		: pos(ppos), varyings(vvar), rhw(rrhw), primf(pprimf) {}

public:
	void rhwInitialize() {
		rhw = 1.0f / pos.w;
		TravMul(varyings, rhw, kTBPerspect);
	}

public:
	AttribList attribs;
	VaryingList varyings;
	Vector4f pos;
	float rhw;
	short primf;
};


template<class A, class V>
void TravPlus(Vertex<A, V>& v1, const Vertex<A, V>& v2, int oper) {
	v1.pos = v1.pos + v2.pos;
	TravPlus(v1.varyings, v2.varyings, oper);
	v1.rhw = v1.rhw + v2.rhw;
}

template<class A, class V>
void TravSub(Vertex<A, V>& v1, const Vertex<A, V>& v2, int oper) {
	v1.pos = v1.pos - v2.pos;
	TravSub(v1.varyings, v2.varyings, oper);
	v1.rhw = v1.rhw - v2.rhw;
}

template<class A, class V>
void TravMul(Vertex<A, V>& v1, float t, int oper) {
	v1.pos = v1.pos * t;
	TravMul(v1.varyings, t, oper);
	v1.rhw = v1.rhw * t;
}

template<class A, class V>
void TravDiv(Vertex<A, V>& v1, float d, int oper) {
	v1.pos = v1.pos / d;
	TravDiv(v1.varyings, d, oper);
	v1.rhw = v1.rhw / d;
}


SHAKURAS_END;