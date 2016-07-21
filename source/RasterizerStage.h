#pragma once
#include "MathAndGeometry.h"
#include "GeometryStage.h"
#include "Vertex.h"
#include "Color.h"
#include <vector>
#include <array>
#include "Sampler.h"


SHAKURAS_BEGIN;


class Edge {
public:
	Edge() {}
	Edge(const Vector3f& vv, const Vector3f& vv1, const Vector3f& vv2) : v(vv), v1(vv1), v2(vv2) {}

public:
	void scanlineInterp(float y) {
		float t = (y - v1.y) / (v2.y - v1.y);
		v = v1 + (v2 - v1) * t;
	}

public:
	Vector3f v, v1, v2;
};


class Trapezoid {
public:
	bool scanlineInterp(float y) {
		left.scanlineInterp(y);
		right.scanlineInterp(y);
		return top <= y && y <= bottom;
	}

public:
	float top, bottom;
	Edge left, right;
};


class Scanline {
public:
	bool initialize(Trapezoid& trap, int yy) {
		bool res = trap.scanlineInterp((float)yy + 0.5f);

		float width = trap.right.v.x - trap.left.v.x;
		x = (int)(trap.left.v.x + 0.5f);
		w = (int)(trap.right.v.x + 0.5f) - x;
		y = yy;
		v = trap.left.v;
		if (trap.left.v.x >= trap.right.v.x) {
			w = 0;
		}
		step = (trap.right.v - trap.left.v) / width;

		return res;
	}

	void next(int xpos) {
		if (x <= xpos) {
			v = v + step;
		}
	}

public:
	Vector3f v, step;
	int x, y, w;
};


class Scanline22 {
public:
	void initialize(Trapezoid& trap, int yy) {
		f0 = s0.initialize(trap, yy);
		f1 = s1.initialize(trap, yy + 1);
	}

	inline int xbegin() const {
		if (f0 && f1) {
			return (std::min)(s0.x, s1.x);
		}
		else if (f0) {
			return s0.x;
		}
		else if (f1) {
			return s1.x;
		}
		return -1;
	}

	int xwidth() const {
		if (f0 && f1) {
			int s = (std::min)(s0.x, s1.x);
			int e = (std::max)(s0.x + s0.w, s1.x + s1.w);
			return e - s;
		}
		else if (f0) {
			return s0.w;
		}
		else if (f1) {
			return s1.w;
		}
		return -1;
	}

	inline bool mask0(int x) const {
		return f0 && s0.x <= x && x < s0.x + s0.w;
	}
	inline bool mask1(int x) const {
		return f1 && s1.x <= x && x < s1.x + s1.w;
	}

	void next(int xpos) {
		s0.next(xpos);
		s1.next(xpos);
	}

public:
	bool f0, f1;
	Scanline s0, s1;
};


template<class V>
inline Vector3f XYRhw(const V& v) {
	return Vector3f(v.pos.x, v.pos.y, v.rhw);
}


template<class V>
int SpliteTrapezoid(const V& v0, const V& v1, const V& v2, std::vector<Trapezoid>& traps) {
	const V* p1 = &v0;
	const V* p2 = &v1;
	const V* p3 = &v2;
	float k, x;

	if (p1->pos.y > p2->pos.y) std::swap(p1, p2);
	if (p1->pos.y > p3->pos.y) std::swap(p1, p3);
	if (p2->pos.y > p3->pos.y) std::swap(p2, p3);
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	traps.push_back(Trapezoid());
	if (p1->pos.y == p2->pos.y) {	// triangle down
		if (p1->pos.x > p2->pos.x) std::swap(p1, p2);
		traps[0].top = p1->pos.y;
		traps[0].bottom = p3->pos.y;
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p3);
		traps[0].right.v1 = XYRhw(*p2);
		traps[0].right.v2 = XYRhw(*p3);
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
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p2);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p3);
		if (traps[0].top < traps[0].bottom) {
			return 1;
		}
		else {
			traps.clear();
			return 0;
		}
	}

	traps.push_back(Trapezoid());
	traps[0].top = p1->pos.y;
	traps[0].bottom = p2->pos.y;
	traps[1].top = p2->pos.y;
	traps[1].bottom = p3->pos.y;

	k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

	if (x <= p3->pos.x) {		// triangle left
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p2);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p3);
		traps[1].left.v1 = XYRhw(*p2);
		traps[1].left.v2 = XYRhw(*p3);
		traps[1].right.v1 = XYRhw(*p1);
		traps[1].right.v2 = XYRhw(*p3);
	}
	else {					// triangle right
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p3);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p2);
		traps[1].left.v1 = XYRhw(*p1);
		traps[1].left.v2 = XYRhw(*p3);
		traps[1].right.v1 = XYRhw(*p2);
		traps[1].right.v2 = XYRhw(*p3);
	}

	return 2;
}


template<class UL, class F, class FS>
class TileShader {
public:
	TileShader() {}

public:
	void process(const UL& u, std::array<F, 4>& tile) {
		sampler_.ddx_ = TexCoord(tile[1].varyings) - TexCoord(tile[0].varyings);
		sampler_.ddy_ = TexCoord(tile[2].varyings) - TexCoord(tile[0].varyings);
		for (int i = 0; i != 4; i++) {
			if (tile[i].draw) {
				fragshader_.process(u, sampler_, tile[i]);
			}
		}
	}

public:
	Sampler sampler_;
	FS fragshader_;
};


template<class UL, class V, class F, class FS>
class RasterizerStage {
public:
	void initialize(int ww, int hh, void* fb) {
		width_ = ww;
		height_ = hh;

		framebuffer_.resize(height_, nullptr);
		zbuffer_.resize(height_);

		for (int y = 0; y != height_; y++) {
			zbuffer_[y].resize(width_, 0.0f);
		}

		char* framebuf = (char*)fb;
		for (int i = 0; i < height_; i++) {
			framebuffer_[i] = (uint32_t*)(framebuf + width_ * 4 * i);
		}
	}

	void process(StageBuffer<UL, V>& buffer) {
		clear();

		//triangle setup, ʡ��

		for (size_t i = 0; i < buffer.vertlist.size();) {
			//triangle traversal
			//fragment shader
			//merging
			if (buffer.vertlist[i].primf == kPFTriangle) {
				const V& v1 = buffer.vertlist[i];
				const V& v2 = buffer.vertlist[i + 1];
				const V& v3 = buffer.vertlist[i + 2];
				drawTriangle(buffer.uniforms, v1, v2, v3);
				i += 3;
				continue;
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
	}

	void drawTriangle(const UL& u, const V& v0, const V& v1, const V& v2) {
		V t0 = v0, t1 = v1, t2 = v2;

		t0.rhwInitialize();
		t1.rhwInitialize();
		t2.rhwInitialize();

		computeTriangleDerivative(t0, t1, t2);

		std::vector<Trapezoid> traps;
		SpliteTrapezoid(t0, t1, t2, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			drawTrapezoid(u, *i);
		}
	}

	void computeTriangleDerivative(const V& v0, const V& v1, const V& v2) {
		V e01 = v1 - v0;
		V e02 = v2 - v0;

		float area = e02.pos.x * e01.pos.y - e01.pos.x * e02.pos.y;
		float inv_area = 1.0f / area;
		
		v0_ = v0;
		ddx_ = (e02 * e01.pos.y - e01 * e02.pos.y) * inv_area;
		ddy_ = (e01 * e02.pos.x - e02 * e01.pos.x) * inv_area;
	}

	F lerpFrag(int x, int y, float rhw) {
		F frag;
		frag.x = x;
		frag.y = y;
		frag.varyings = (v0_.varyings + ddx_.varyings * (x - v0_.pos.x) + ddy_.varyings * (y - v0_.pos.y)) / rhw;
		frag.z = rhw;

		return frag;
	}

	void drawScanline(const UL& u, Scanline22& s22) {
		int x = s22.xbegin();
		int w = s22.xwidth();
		for (; w > 0; x += 2, w -= 2) {
			if (x >= 0 && x < width_) {
				float rhw0 = s22.s0.v.z;
				float rhw1 = s22.s1.v.z;

				// 2, 3
				// 0, 1
				std::array<F, 4> tile;
				tile[0] = lerpFrag(x, s22.s0.y, rhw0);
				tile[0].draw = s22.mask0(x);
				tile[2] = lerpFrag(x, s22.s1.y, rhw1);
				tile[2].draw = s22.mask1(x);

				s22.next(x);
				rhw0 = s22.s0.v.z;
				rhw1 = s22.s1.v.z;

				tile[1] = lerpFrag(x + 1, s22.s0.y, rhw0);
				tile[1].draw = s22.mask0(x + 1);
				tile[3] = lerpFrag(x + 1, s22.s1.y, rhw1);
				tile[3].draw = s22.mask1(x + 1);

				s22.next(x + 1);

				//fragment shader
				tileshader_.process(u, tile);

				//merging
				for (size_t i = 0; i != 4; i++) {
					const F& frag = tile[i];
					if (frag.draw && frag.z > zbuffer_[frag.y][frag.x]) {
						zbuffer_[frag.y][frag.x] = frag.z;
						framebuffer_[frag.y][frag.x] = IRgba(frag.c);
					}
				}
			}

			if (x >= width_) {
				break;
			}
		}
	}

	void drawTrapezoid(const UL& u, Trapezoid& trap) {
		Scanline22 scanline;
		int j, top, bottom;
		top = (int)(trap.top + 0.5f);
		bottom = (int)(trap.bottom + 0.5f);
		for (j = top; j < bottom; j += 2) {
			if (j >= 0 && j < height_) {
				scanline.initialize(trap, j);
				drawScanline(u, scanline);
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
	V ddx_, ddy_;
	V v0_;
	TileShader<UL, F, FS> tileshader_;
};


SHAKURAS_END;