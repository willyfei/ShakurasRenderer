#include "stdafx.h"
#include "GsDevice.h"


SHAKURAS_BEGIN;


int SpliteTrapezoid(const GsTriangle& tri, std::vector<GsTrapezoid>& traps) {
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


inline int CheckCVV(const Vector4f& v) {
	int check = 0;
	if (v.z < 0.0f) check |= 1;
	if (v.z > v.w) check |= 2;
	if (v.x < -v.w) check |= 4;
	if (v.x > v.w) check |= 8;
	if (v.y < -v.w) check |= 16;
	if (v.y > v.w) check |= 32;
	return check;
}


void GsDevice::initialize(int ww, int hh, void* fb) {
	w = ww;
	h = hh;

	framebuffer.resize(h, nullptr);
	zbuffer.resize(h, std::vector<float>(w, 0.0f));

	char* framebuf = (char*)fb;
	for (int i = 0; i < h; i++) {
		framebuffer[i] = (uint32_t*)(framebuf + w * 4 * i);
	}
}

void GsDevice::drawScanline(GsScanline& scanline, GsStatePtr state)
{
	assert(state);
	uint32_t* framebuf = framebuffer[scanline.y];
	std::vector<float>& zbuf = zbuffer[scanline.y];
	int x = scanline.x;
	int ww = scanline.w;
	for (; w > 0; x++, ww--) {
		if (x >= 0 && x < w) {
			float rhw = scanline.v.rhw;
			if (rhw > zbuf[x]) {
				float www = 1.0f / rhw;
				zbuf[x] = rhw;
				if (state->drawcolor) {
					float r = scanline.v.color.r * www;
					float g = scanline.v.color.g * www;
					float b = scanline.v.color.b * www;
					int R = int(r * 255.0f);
					int G = int(g * 255.0f);
					int B = int(b * 255.0f);
					R = Restrict(R, 0, 255);
					G = Restrict(G, 0, 255);
					B = Restrict(B, 0, 255);
					framebuf[x] = (R << 16) | (G << 8) | B;
				}
				if (state->drawtexture && state->texture) {
					float u = scanline.v.tc.u * www;
					float v = scanline.v.tc.v * www;
					uint32_t cc = state->texture->at(u, v);
					framebuf[x] = cc;
				}
			}
		}
		scanline.v = scanline.v + scanline.step;
		if (x >= w) {
			break;
		}
	}
}

void GsDevice::renderPrimitive(const GsTriangle& tri, GsTransformerPtr trsf, GsStatePtr state)
{
	assert(trsf);
	assert(state);

	const GsVertex& v1 = tri[0];
	const GsVertex& v2 = tri[1];
	const GsVertex& v3 = tri[2];

	Vector4f p1, p2, p3, c1, c2, c3;

	c1 = trsf->wvp.transform(v1.pos);
	c2 = trsf->wvp.transform(v2.pos);
	c3 = trsf->wvp.transform(v3.pos);

	if (CheckCVV(c1) != 0 && CheckCVV(c2) != 0 && CheckCVV(c3) != 0) {
		return;
	}

	p1 = trsf->homogenize(c1);
	p2 = trsf->homogenize(c2);
	p3 = trsf->homogenize(c3);

	if (state->drawcolor || state->drawtexture) {
		GsVertex t1 = v1, t2 = v2, t3 = v3;

		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;

		t1.rhwInitialize();
		t2.rhwInitialize();
		t3.rhwInitialize();

		std::vector<GsTrapezoid> traps;
		SpliteTrapezoid({ t1, t2, t3 }, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			drawTrapezoid(*i, state);
		}
	}

	if (state->drawwireframe) {
		drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, state->wirecolor.u32());
		drawLine((int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, state->wirecolor.u32());
		drawLine((int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, state->wirecolor.u32());
	}
}

void GsDevice::renderPrimitive(const GsLine& line, GsTransformerPtr trsf, GsStatePtr state) {
	assert(trsf);
	assert(state);

	const GsVertex& v1 = line[0];
	const GsVertex& v2 = line[1];

	Vector4f p1, p2, c1, c2;

	c1 = trsf->wvp.transform(v1.pos);
	c2 = trsf->wvp.transform(v2.pos);

	if (CheckCVV(c1) != 0 && CheckCVV(c2) != 0) {
		return;
	}

	p1 = trsf->homogenize(c1);
	p2 = trsf->homogenize(c2);

	if (state->drawcolor) {
		drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, v1.color.u32());
	}
}

void GsDevice::drawTrapezoid(GsTrapezoid& trap, GsStatePtr state)
{
	GsScanline scanline;
	int j, top, bottom;
	top = (int)(trap.top + 0.5f);
	bottom = (int)(trap.bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < h) {
			trap.scanlineInterp((float)j + 0.5f);
			scanline.initialize(trap, j);
			drawScanline(scanline, state);
		}
		if (j >= h) {
			break;
		}
	}
}

void GsDevice::drawLine(int x1, int y1, int x2, int y2, uint32_t c)
{
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

void GsDevice::drawPixel(int x, int y, uint32_t c)
{
	if (((uint32_t)x) < (uint32_t)w && ((uint32_t)y) < (uint32_t)h) {
		framebuffer[y][x] = c;
	}
}

void GsDevice::clear() {
	int y, x;
	for (y = 0; y < h; y++) {
		uint32_t *dst = framebuffer[y];
		uint32_t cc = (h - 1 - y) * 230 / (h - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		for (x = w; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < h; y++) {
		std::vector<float>& dst = zbuffer[y];
		std::fill(dst.begin(), dst.end(), 0.0f);
	}
}


SHAKURAS_END;