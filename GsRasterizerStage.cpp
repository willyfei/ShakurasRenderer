#include "stdafx.h"
#include "GsRasterizerStage.h"
#include <assert.h>


SHAKURAS_BEGIN;


class GsEdge {
public:
	GsEdge() {}
	GsEdge(const GsVertex& vv, const GsVertex& vv1, const GsVertex& vv2) : v(vv), v1(vv1), v2(vv2) {}

public:
	void scanlineInterp(float y) {
		float t = (y - v1.pos.y) / (v2.pos.y - v1.pos.y);
		v = Interp(v1, v2, t);
	}

public:
	GsVertex v, v1, v2;
};


class GsTrapezoid {
public:
	void scanlineInterp(float y) {
		left.scanlineInterp(y);
		right.scanlineInterp(y);
	}

public:
	float top, bottom;
	GsEdge left, right;
};


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
		step = (trap.right.v - trap.left.v) / width;
	}

public:
	GsVertex v, step;
	int x, y, w;
};


int SpliteTrapezoid(const std::array<GsVertex, 3>& tri, std::vector<GsTrapezoid>& traps) {
	const GsVertex* p1 = &tri[0];
	const GsVertex* p2 = &tri[1];
	const GsVertex* p3 = &tri[2];
	float k, x;

	if (p1->pos.y > p2->pos.y) std::swap(p1, p2);
	if (p1->pos.y > p3->pos.y) std::swap(p1, p3);
	if (p2->pos.y > p3->pos.y) std::swap(p2, p3);
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	traps.push_back(GsTrapezoid());
	if (p1->pos.y == p2->pos.y) {	// triangle down
		if (p1->pos.x > p2->pos.x) std::swap(p1, p2);
		traps[0].top = p1->pos.y;
		traps[0].bottom = p3->pos.y;
		traps[0].left.v1 = *p1;
		traps[0].left.v2 = *p3;
		traps[0].right.v1 = *p2;
		traps[0].right.v2 = *p3;
		if (traps[0].top < traps[0].bottom) {
			return 1;
		}
		else {
			traps.clear();
			return 0;
		}
	}

	if (p2->pos.y == p3->pos.y) {	// triangle up
		if (p2->pos.x > p3->pos.x) std::swap(p2, p3);
		traps[0].top = p1->pos.y;
		traps[0].bottom = p3->pos.y;
		traps[0].left.v1 = *p1;
		traps[0].left.v2 = *p2;
		traps[0].right.v1 = *p1;
		traps[0].right.v2 = *p3;
		if (traps[0].top < traps[0].bottom) {
			return 1;
		}
		else {
			traps.clear();
			return 0;
		}
	}

	traps.push_back(GsTrapezoid());
	traps[0].top = p1->pos.y;
	traps[0].bottom = p2->pos.y;
	traps[1].top = p2->pos.y;
	traps[1].bottom = p3->pos.y;

	k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

	if (x <= p3->pos.x) {		// triangle left
		traps[0].left.v1 = *p1;
		traps[0].left.v2 = *p2;
		traps[0].right.v1 = *p1;
		traps[0].right.v2 = *p3;
		traps[1].left.v1 = *p2;
		traps[1].left.v2 = *p3;
		traps[1].right.v1 = *p1;
		traps[1].right.v2 = *p3;
	}
	else {					// triangle right
		traps[0].left.v1 = *p1;
		traps[0].left.v2 = *p3;
		traps[0].right.v1 = *p1;
		traps[0].right.v2 = *p2;
		traps[1].left.v1 = *p1;
		traps[1].left.v2 = *p3;
		traps[1].right.v1 = *p2;
		traps[1].right.v2 = *p3;
	}

	return 2;
}


void GsRasterizerStage::initialize(int ww, int hh, void* fb) {
	width_ = ww;
	height_ = hh;

	framebuffer_.resize(height_, nullptr);
	zbuffer_.resize(height_, std::vector<float>(width_, 0.0f));

	char* framebuf = (char*)fb;
	for (int i = 0; i < height_; i++) {
		framebuffer_[i] = (uint32_t*)(framebuf + width_ * 4 * i);
	}
}

void GsRasterizerStage::drawScanline(GsScanline& scanline, GsTextureU32Ptr texture) {
	assert(texture);
	uint32_t* framebuf = framebuffer_[scanline.y];
	std::vector<float>& zbuf = zbuffer_[scanline.y];
	int x = scanline.x;
	int w = scanline.w;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width_) {
			float rhw = scanline.v.rhw;
			if (rhw > zbuf[x]) {
				float ww = 1.0f / rhw;
				zbuf[x] = rhw;
				float u = scanline.v.tc.u * ww;
				float v = scanline.v.tc.v * ww;
				uint32_t cc = texture->at(u, v);
				framebuf[x] = cc;
			}
		}
		scanline.v = scanline.v + scanline.step;
		if (x >= width_) {
			break;
		}
	}
}

void GsRasterizerStage::renderPrimitive(const GsVertex& v1, const GsVertex& v2, const GsVertex& v3, GsTextureU32Ptr texture) {
	assert(texture);

	GsVertex t1 = v1, t2 = v2, t3 = v3;

	t1.rhwInitialize();
	t2.rhwInitialize();
	t3.rhwInitialize();

	std::vector<GsTrapezoid> traps;
	SpliteTrapezoid({ t1, t2, t3 }, traps);

	for (auto i = traps.begin(); i != traps.end(); i++) {
		drawTrapezoid(*i, texture);
	}
}

void GsRasterizerStage::renderPrimitive(const GsVertex& v1, const GsVertex& v2) {
	drawLine((int)v1.pos.x, (int)v1.pos.y, (int)v2.pos.x, (int)v2.pos.y, v1.color.u32());
}

void GsRasterizerStage::drawTrapezoid(GsTrapezoid& trap, GsTextureU32Ptr texture) {
	GsScanline scanline;
	int j, top, bottom;
	top = (int)(trap.top + 0.5f);
	bottom = (int)(trap.bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < height_) {
			trap.scanlineInterp((float)j + 0.5f);
			scanline.initialize(trap, j);
			drawScanline(scanline, texture);
		}
		if (j >= height_) {
			break;
		}
	}
}

void GsRasterizerStage::drawLine(int x1, int y1, int x2, int y2, uint32_t c) {
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {
		drawPixel(x1, y1, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) drawPixel(x1, y, c);
		drawPixel(x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) drawPixel(x, y1, c);
		drawPixel(x2, y2, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				drawPixel(x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					drawPixel(x, y, c);
				}
			}
			drawPixel(x2, y2, c);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				drawPixel(x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					drawPixel(x, y, c);
				}
			}
			drawPixel(x2, y2, c);
		}
	}
}

void GsRasterizerStage::drawPixel(int x, int y, uint32_t c) {
	if (((uint32_t)x) < (uint32_t)width_ && ((uint32_t)y) < (uint32_t)height_) {
		framebuffer_[y][x] = c;
	}
}

void GsRasterizerStage::clear() {
	int y, x;
	for (y = 0; y < height_; y++) {
		uint32_t *dst = framebuffer_[y];
		uint32_t cc = (height_ - 1 - y) * 230 / (height_ - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		for (x = width_; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < height_; y++) {
		std::vector<float>& dst = zbuffer_[y];
		std::fill(dst.begin(), dst.end(), 0.0f);
	}
}

void GsRasterizerStage::process(In& input) {
	clear();
	for (int i = 0; i != input.itris.size(); i++) {
		renderPrimitive(input.vertlist[input.itris[i][0]], input.vertlist[input.itris[i][1]], 
			input.vertlist[input.itris[i][2]], input.texturelist[input.itexs[i]]);
	}
}


SHAKURAS_END;