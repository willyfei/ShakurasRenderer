#pragma once
#include "MathAndGeometry.h"
#include "GsGeometryStage.h"
#include "GsVertexFragment.h"
#include "GsColor.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


template<class Vert>
class GsEdge {
public:
	GsEdge() {}
	GsEdge(const Vert& vv, const Vert& vv1, const Vert& vv2) : v(vv), v1(vv1), v2(vv2) {}

public:
	void scanlineInterp(float y) {
		float t = (y - v1.pos.y) / (v2.pos.y - v1.pos.y);
		v = v2;
		LerpSub(v, v1);//v = v2 - v1
		LerpMul(v, t);//v = (v2 - v1) * t
		LerpPlus(v, v1);//v = v1 + (v2 - v1) * t;
	}

public:
	Vert v, v1, v2;
};


template<class Vert>
class GsTrapezoid {
public:
	void scanlineInterp(float y) {
		left.scanlineInterp(y);
		right.scanlineInterp(y);
	}

public:
	float top, bottom;
	GsEdge<Vert> left, right;
};


template<class Vert>
class GsScanline {
public:
	void initialize(const GsTrapezoid<Vert>& trap, int yy) {
		float width = trap.right.v.pos.x - trap.left.v.pos.x;
		x = (int)(trap.left.v.pos.x + 0.5f);
		w = (int)(trap.right.v.pos.x + 0.5f) - x;
		y = yy;
		v = trap.left.v;
		if (trap.left.v.pos.x >= trap.right.v.pos.x) {
			w = 0;
		}
		step = trap.right.v;
		LerpSub(step, trap.left.v);//step = trap.right.v - trap.left.v
		LerpMul(step, 1.0f / width);//step = (trap.right.v - trap.left.v) / width
	}

public:
	Vert v, step;
	int x, y, w;
};


template<class Vert>
int SpliteTrapezoid(const std::array<Vert, 3>& tri, std::vector<GsTrapezoid<Vert> >& traps) {
	const Vert* p1 = &tri[0];
	const Vert* p2 = &tri[1];
	const Vert* p3 = &tri[2];
	float k, x;

	if (p1->pos.y > p2->pos.y) std::swap(p1, p2);
	if (p1->pos.y > p3->pos.y) std::swap(p1, p3);
	if (p2->pos.y > p3->pos.y) std::swap(p2, p3);
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	traps.push_back(GsTrapezoid<Vert>());
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

	traps.push_back(GsTrapezoid<Vert>());
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


template<class UniformList, class Vert, class Frag, class FragShader>
class GsRasterizerStage {
public:
	void initialize(int ww, int hh, void* fb) {
		width_ = ww;
		height_ = hh;

		fraglist_.clear();
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

		fragshader_ = std::make_shared<FragShader>();
	}

	void process(GsStageBuffer<UniformList, Vert>& buffer) {
		clear();

		//triangle setup, Ê¡ÂÔ

		//triangle traversal
		size_t i = 0;
		while (i < buffer.vertlist.size()) {
			if (buffer.vclist[i] == kVCTriangleBegin) {
				const Vert& v1 = buffer.vertlist[i];
				const Vert& v2 = buffer.vertlist[i + 1];
				const Vert& v3 = buffer.vertlist[i + 2];
				traversalTriangle({ v1, v2, v3 });
				i += 3;
			}
		}

		//fragment shader
		for (auto i = fraglist_.begin(); i != fraglist_.end(); i++) {
			fragshader_->process(buffer.uniforms, *i);
		}

		//merging
		for (auto i = fraglist_.begin(); i != fraglist_.end(); i++) {
			Frag& frag = *i;
			if (frag.z > zbuffer_[frag.y][frag.x]) {
				zbuffer_[frag.y][frag.x] = frag.z;
				framebuffer_[frag.y][frag.x] = IRgba(frag.c);
			}
		}
	}

private:
	void clear() {
		for (int y = 0; y < height_; y++) {
			uint32_t *dst = framebuffer_[y];
			uint32_t cc = IBgr(Vector3f(0.2f, 0.2f, 0.6f));
			for (int x = width_; x > 0; dst++, x--) dst[0] = cc;
		}

		for (int y = 0; y < height_; y++) {
			std::vector<float>& dst = zbuffer_[y];
			std::fill(dst.begin(), dst.end(), 0.0f);
		}

		fraglist_.clear();
		fraglist_.reserve(width_ * height_);
		for (int y = 0; y != height_; y++) {
			for (int x = 0; x != width_; x++) {
				fragbuffer_[y][x].clear();
			}
		}
	}

	void traversalTriangle(const std::array<Vert, 3>& tri) {
		Vert t1 = tri[0], t2 = tri[1], t3 = tri[2];

		t1.rhwInitialize();
		t2.rhwInitialize();
		t3.rhwInitialize();

		std::vector<GsTrapezoid<Vert> > traps;
		SpliteTrapezoid({ t1, t2, t3 }, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			traversalTrapezoid(*i);
		}
	}

	void traversalScanline(GsScanline<Vert>& scanline) {
		int x = scanline.x;
		int w = scanline.w;
		for (; w > 0; x++, w--) {
			if (x >= 0 && x < width_) {
				float rhw = scanline.v.rhw;
				float ww = 1.0f / rhw;

				Frag frag;
				frag.x = x;
				frag.y = scanline.y;
				frag.varyings = scanline.v.varyings;
				PerspectMul(frag.varyings, ww);
				frag.z = rhw;

				fragbuffer_[scanline.y][x].push_back((int)fraglist_.size());
				fraglist_.push_back(frag);
			}
			LerpPlus(scanline.v, scanline.step);
			if (x >= width_) {
				break;
			}
		}
	}

	void traversalTrapezoid(GsTrapezoid<Vert>& trap) {
		GsScanline<Vert> scanline;
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

private:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
	std::vector<Frag> fraglist_;
	std::vector<std::vector<std::vector<int> > > fragbuffer_;
	std::shared_ptr<FragShader> fragshader_;
};


SHAKURAS_END;