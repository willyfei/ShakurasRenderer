#pragma once
#include "UtilityAndMath.h"
#include "GsPrimitive.h"
#include <vector>
#include <assert.h>
#include "GsTransformer.h"


SHAKURAS_BEGIN;


class GsTexture {
public:
	GsTexture(void* bits, int ww, int hh) {
		reset(bits, ww, hh);
	}

public:
	void reset(void* bits, int ww, int hh) {
		char* ptr = (char*)bits;
		for (int i = 0; i < h; ptr += h * 4, i++) {
			buffer[i] = (uint32_t*)ptr;
		}
		w = ww;
		h = hh;
	}

	virtual uint32_t at(float u, float v) {
		u *= (w - 1);
		v *= (h - 1);
		int x = (int)(u + 0.5f);
		int y = (int)(v + 0.5f);
		x = Restrict(x, 0, w - 1);
		y = Restrict(y, 0, h - 1);
		return buffer[y][x];
	}

public:
	uint32_t** buffer;
	int w, h;
};


SHAKURAS_SHARED_PTR(GsTexture);


class GsState {
public:
	GsState() : drawwireframe(true), drawcolor(false), drawtexture(false), wirecolor(1.0f, 1.0f, 1.0f){}

public:
	bool drawwireframe;
	bool drawcolor;
	bool drawtexture;
	GsTexturePtr texture;
	GsColor24 wirecolor;
};


SHAKURAS_SHARED_PTR(GsState);



class GsScanline {
public:
	void initialize(const GsTrapezoid& trap, int yy) {
		float width = trap.right.v.pos.x - trap.left.v.pos.x;
		x = (int)(trap.left.v.pos.x + 0.5f);
		w = (int)(trap.right.v.pos.x + 0.5f) - x;
		y = yy;
		v = trap.left.v;
		if (trap.left.v.pos.x >= trap.right.v.pos.x) {
			w = 0;
		}
		step = (trap.left.v - trap.right.v) / width;
	}

public:
	GsVertex v, step;
	int x, y, w;
};


class GsDevice {
public:
	GsDevice() : memptr(nullptr), framebuffer(nullptr), zbuffer(nullptr) {}

public:
	void initialize(int ww, int hh, void* fb);

	void destory();

	void clear(uint32_t color);

	void drawPixel(int x, int y, uint32_t c);

	void drawLine(int x1, int y1, int x2, int y2, uint32_t c);

	void drawScanline(GsScanline& scanline, GsStatePtr state);

	void drawTrapezoid(GsTrapezoid& trap, GsStatePtr state);

	void renderPrimitive(const GsTriangle& tri, GsTransformerPtr trsf, GsStatePtr state);

	void renderPrimitive(const GsLine& line, GsTransformerPtr trsf, GsStatePtr state);

public:
	char* memptr;
	uint32_t** framebuffer;
	float** zbuffer;
	int w, h;
};


SHAKURAS_END;