#pragma once
#include "UtilityAndMath.h"
#include "GsPrimitive.h"
#include <vector>
#include <assert.h>
#include "GsTransformer.h"
#include "GsTexture.h"


SHAKURAS_BEGIN;


class GsState {
public:
	GsState() : drawwireframe(true), drawcolor(false), drawtexture(false), wirecolor(1.0f, 1.0f, 1.0f){}
	GsState(bool dwf, bool dc, bool dt, GsTextureU32Ptr t, GsColor24 c) 
		: drawwireframe(dwf), drawcolor(dc), drawtexture(dt), texture(t), wirecolor(c) {}

public:
	bool drawwireframe;
	bool drawcolor;
	bool drawtexture;
	GsTextureU32Ptr texture;
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
	GsDevice() : w(1), h(1) {}

public:
	void initialize(int ww, int hh, void* fb);

	void clear();

	void drawPixel(int x, int y, uint32_t c);

	void drawLine(int x1, int y1, int x2, int y2, uint32_t c);

	void drawScanline(GsScanline& scanline, GsStatePtr state);

	void drawTrapezoid(GsTrapezoid& trap, GsStatePtr state);

	void renderPrimitive(const GsTriangle& tri, GsTransformerPtr trsf, GsStatePtr state);

	void renderPrimitive(const GsLine& line, GsTransformerPtr trsf, GsStatePtr state);

public:
	std::vector<uint32_t*> framebuffer;
	std::vector<std::vector<float> > zbuffer;
	int w, h;
};


SHAKURAS_END;