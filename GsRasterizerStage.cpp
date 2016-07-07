#include "stdafx.h"
#include "GsRasterizerStage.h"


SHAKURAS_BEGIN;


class GsEdge {
public:
	GsEdge() {}
	GsEdge(const GsVertexA4& vv, const GsVertexA4& vv1, const GsVertexA4& vv2) : v(vv), v1(vv1), v2(vv2) {}

public:
	void scanlineInterp(float y) {
		float t = (y - v1.pos.y) / (v2.pos.y - v1.pos.y);
		v = Interp(v1, v2, t);
	}

public:
	GsVertexA4 v, v1, v2;
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
	GsVertexA4 v, step;
	int x, y, w;
};


int SpliteTrapezoid(const std::array<GsVertexA4, 3>& tri, std::vector<GsTrapezoid>& traps) {
	const GsVertexA4* p1 = &tri[0];
	const GsVertexA4* p2 = &tri[1];
	const GsVertexA4* p3 = &tri[2];
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
	zbuffer_.resize(height_);
	fragbuffer_.resize(height_);

	for (int y = 0; y != height_; y++) {
		zbuffer_[y].resize(width_, 0.0f);
		fragbuffer_[y].resize(width_);
		for (int x = 0; x != width_; x++) {
			fragbuffer_[y][x].clear();
		}
	}

	char* framebuf = (char*)fb;
	for (int i = 0; i < height_; i++) {
		framebuffer_[i] = (uint32_t*)(framebuf + width_ * 4 * i);
	}
}


void GsRasterizerStage::traversalScanline(GsScanline& scanline) {
	int x = scanline.x;
	int w = scanline.w;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width_) {
			float rhw = scanline.v.rhw;
			float ww = 1.0f / rhw;

			GsFragmentV4 frag;
			frag.x = x;
			frag.y = scanline.y;
			frag.varying = scanline.v.attrib * ww;
			frag.z = rhw;

			fragbuffer_[scanline.y][x].push_back(frag);
		}
		scanline.v = scanline.v + scanline.step;
		if (x >= width_) {
			break;
		}
	}
}


void GsRasterizerStage::traversalTriangle(const std::array<GsVertexA4, 3>& tri) {
	GsVertexA4 t1 = tri[0], t2 = tri[1], t3 = tri[2];

	t1.rhwInitialize();
	t2.rhwInitialize();
	t3.rhwInitialize();

	std::vector<GsTrapezoid> traps;
	SpliteTrapezoid({ t1, t2, t3 }, traps);

	for (auto i = traps.begin(); i != traps.end(); i++) {
		traversalTrapezoid(*i);
	}
}


void GsRasterizerStage::traversalTrapezoid(GsTrapezoid& trap) {
	GsScanline scanline;
	int j, top, bottom;
	top = (int)(trap.top + 0.5f);
	bottom = (int)(trap.bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < height_) {
			trap.scanlineInterp((float)j + 0.5f);
			scanline.initialize(trap, j);
			traversalScanline(scanline);
		}
		if (j >= height_) {
			break;
		}
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

	for (int y = 0; y != height_; y++) {
		for (int x = 0; x != width_; x++) {
			fragbuffer_[y][x].clear();
		}
	}
}


void GsRasterizerStage::process(In& input) {
	clear();

	//triangle setup, Ê¡ÂÔ

	//triangle traversal
	for (int i = 0; i != input.itris.size(); i++) {
		const GsVertexA4& v1 = input.vertlist[input.itris[i][0]];
		const GsVertexA4& v2 = input.vertlist[input.itris[i][1]];
		const GsVertexA4& v3 = input.vertlist[input.itris[i][2]];

		traversalTriangle({ v1, v2, v3 });
	}

	//fragment shader
	for (auto i = fragbuffer_.begin(); i != fragbuffer_.end(); i++) {
		for (auto ii = i->begin(); ii != i->end(); ii++) {
			for (auto iii = ii->begin(); iii != ii->end(); iii++) {
				GsFragmentV4& frag = *iii;
				GsTextureU32Ptr texture = input.texture;
				frag.c = texture->at(frag.varying[0], frag.varying[1]);
			}
		}
	}

	//merging
	for (auto i = fragbuffer_.begin(); i != fragbuffer_.end(); i++) {
		for (auto ii = i->begin(); ii != i->end(); ii++) {
			for (auto iii = ii->begin(); iii != ii->end(); iii++) {
				GsFragmentV4& frag = *iii;
				if (frag.z > zbuffer_[frag.y][frag.x]) {
					zbuffer_[frag.y][frag.x] = frag.z;
					framebuffer_[frag.y][frag.x] = frag.c;
				}
			}
		}
	}
}


SHAKURAS_END;