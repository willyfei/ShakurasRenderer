#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


class GsColor24 {
public:
	GsColor24() : r(0.0f), g(0.0f), b(0.0f) {}
	GsColor24(float rr, float gg, float bb) : r(rr), g(gg), b(bb) {}

public:
	uint32_t u32() const {
		uint32_t u = 0;
		unsigned char* pu = (unsigned char*)&u;
		pu[0] = (int)(r * 255);
		pu[1] = (int)(g * 255);
		pu[2] = (int)(b * 255);
		pu[3] = 255;
		return u;
	}

public:
	float r, g, b;
};


inline GsColor24 operator+(const GsColor24& c1, const GsColor24& c2) {
	return GsColor24(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
inline GsColor24 operator-(const GsColor24& c1, const GsColor24& c2) {
	return GsColor24(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
}
inline GsColor24 operator*(const GsColor24& c1, float t) {
	return GsColor24(c1.r * t, c1.g * t, c1.b * t);
}
inline GsColor24 operator/(const GsColor24& c1, float d) {
	float inv = 1.0f / d;
	return GsColor24(c1.r * inv, c1.g * inv, c1.b * inv);
}


class GsTexCoord {
public:
	GsTexCoord() : u(0.0f), v(0.0f) {}
	GsTexCoord(float uu, float vv) : u(uu), v(vv) {}

public:
	float u, v;
};


inline GsTexCoord operator+(const GsTexCoord& tc1, const GsTexCoord& tc2) {
	return GsTexCoord(tc1.u + tc2.u, tc1.v + tc2.v);
}
inline GsTexCoord operator-(const GsTexCoord& tc1, const GsTexCoord& tc2) {
	return GsTexCoord(tc1.u - tc2.u, tc1.v - tc2.v);
}
inline GsTexCoord operator*(const GsTexCoord& tc1, float t) {
	return GsTexCoord(tc1.u * t, tc1.v * t);
}
inline GsTexCoord operator/(const GsTexCoord& tc1, float d) {
	float t = 1.0f / d;
	return GsTexCoord(tc1.u * t, tc1.v * t);
}


class GsVertex {
public:
	GsVertex() : rhw(1.0f) {}
	GsVertex(const Vector4f& ppos, const GsTexCoord& ttc, const GsColor24& cc, float rrhw)
		: pos(ppos), tc(ttc), color(cc), rhw(rrhw) {}

public:
	void rhwInitialize() {
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


inline GsVertex operator+(const GsVertex& v1, const GsVertex& v2) {
	return GsVertex(v1.pos + v2.pos, v1.tc + v2.tc, v1.color + v2.color, v1.rhw + v2.rhw);
}
inline GsVertex operator-(const GsVertex& v1, const GsVertex& v2) {
	return GsVertex(v1.pos - v2.pos, v1.tc - v2.tc, v1.color - v2.color, v1.rhw - v2.rhw);
}
inline GsVertex operator*(const GsVertex& v1, float t) {
	return GsVertex(v1.pos * t, v1.tc * t, v1.color * t, v1.rhw * t);
}
inline GsVertex operator/(const GsVertex& v1, float d) {
	float t = 1.0f / d;
	return GsVertex(v1.pos * t, v1.tc * t, v1.color * t, v1.rhw * t);
}


SHAKURAS_END;