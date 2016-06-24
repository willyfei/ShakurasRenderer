#pragma once
#include "UtilityAndMath.h"


SHAKURAS_BEGIN;


class GsColor24 {
public:
	GsColor24() : r(0.0f), g(0.0f), b(0.0f) {}
	GsColor24(float rr, float gg, float bb) : r(rr), g(gg), b(bb) {}

public:
	float r, g, b;
};


class GsTexCoord {
public:
	GsTexCoord() : u(0.0f), v(0.0f) {}
	GsTexCoord(float uu, float vv) : u(uu), v(vv) {}

public:
	float u, v;
};


class GsVertex {
public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos, const GsTexCoord& ttc, const GsColor24& cc, float rrhw)
		: pos(ppos), tc(ttc), color(cc), rhw(rrhw) {}

public:
	void rhwInit() {
		rhw = 1.0f / pos.w;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	}

public:
	Vector4f pos;
	GsTexCoord tc;
	GsColor24 color;
	float rhw;
};



SHAKURAS_END;