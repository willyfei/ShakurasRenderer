#pragma once
#include "MathAndGeometry.h"
#include "GeometryStage.h"
#include "Vertex.h"
#include "Color.h"
#include <vector>
#include <array>


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
	void scanlineInterp(float y) {
		left.scanlineInterp(y);
		right.scanlineInterp(y);
	}

public:
	float top, bottom;
	Edge left, right;
};


class Scanline {
public:
	void initialize(const Trapezoid& trap, int yy) {
		float width = trap.right.v.x - trap.left.v.x;
		x = (int)(trap.left.v.x + 0.5f);
		w = (int)(trap.right.v.x + 0.5f) - x;
		y = yy;
		v = trap.left.v;
		if (trap.left.v.x >= trap.right.v.x) {
			w = 0;
		}
		step = trap.right.v;
		step = (trap.right.v - trap.left.v) / width;
	}

public:
	Vector3f v, step;
	int x, y, w;
};


template<class Vert>
inline Vector3f XYRhw(const Vert& v) {
	return Vector3f(v.pos.x, v.pos.y, v.rhw);
}

template<class Vert>
int SpliteTrapezoid(const Vert& v0, const Vert& v1, const Vert& v2, std::vector<Trapezoid>& traps) {
	const Vert* p1 = &v0;
	const Vert* p2 = &v1;
	const Vert* p3 = &v2;
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


template<class UniformList, class Vert, class Frag, class FragShader>
class RasterizerStage {
public:
	void initialize(int ww, int hh, void* fb) {
		width_ = ww;
		height_ = hh;

		fraglist_.clear();
		framebuffer_.resize(height_, nullptr);
		zbuffer_.resize(height_);

		for (int y = 0; y != height_; y++) {
			zbuffer_[y].resize(width_, 0.0f);
		}

		char* framebuf = (char*)fb;
		for (int i = 0; i < height_; i++) {
			framebuffer_[i] = (uint32_t*)(framebuf + width_ * 4 * i);
		}

		fragshader_ = std::make_shared<FragShader>();
	}

	void process(StageBuffer<UniformList, Vert>& buffer) {
		clear();

		//triangle setup, Ê¡ÂÔ

		for (size_t i = 0; i < buffer.vertlist.size();) {
			//triangle traversal
			if (buffer.vertlist[i].primf == kPFTriangle) {
				const Vert& v1 = buffer.vertlist[i];
				const Vert& v2 = buffer.vertlist[i + 1];
				const Vert& v3 = buffer.vertlist[i + 2];
				traversalTriangle(v1, v2, v3 );
				i += 3;
			}

			//fragment shader
			for (auto ii = fraglist_.begin(); ii != fraglist_.end(); ii++) {
				fragshader_->process(buffer.uniforms, *ii);
			}

			//merging
			for (auto ii = fraglist_.begin(); ii != fraglist_.end(); ii++) {
				Frag& frag = *ii;
				if (frag.z > zbuffer_[frag.y][frag.x]) {
					zbuffer_[frag.y][frag.x] = frag.z;
					framebuffer_[frag.y][frag.x] = IRgba(frag.c);
				}
			}

			fraglist_.clear();
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
	}

	void traversalTriangle(const Vert& v0, const Vert& v1, const Vert& v2) {
		Vert t0 = v0, t1 = v1, t2 = v2;

		t0.rhwInitialize();
		t1.rhwInitialize();
		t2.rhwInitialize();

		computeTriangleDerivative(t0, t1, t2);

		std::vector<Trapezoid> traps;
		SpliteTrapezoid(t0, t1, t2, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			traversalTrapezoid(*i);
		}
	}

	void computeTriangleDerivative(const Vert& v0, const Vert& v1, const Vert& v2) {
		Vert e01 = v1 - v0;
		Vert e02 = v2 - v0;

		float area = e02.pos.x * e01.pos.y - e01.pos.x * e02.pos.y;
		float inv_area = 1.0f / area;
		
		v0_ = v0;
		ddx_ = (e02 * e01.pos.y - e01 * e02.pos.y) * inv_area;
		ddy_ = (e01 * e02.pos.x - e02 * e01.pos.x) * inv_area;
	}

	void traversalScanline(Scanline& scanline) {
		int x = scanline.x;
		int w = scanline.w;
		for (; w > 0; x++, w--) {
			if (x >= 0 && x < width_) {
				float rhw = scanline.v.z;
				float ww = 1.0f / rhw;

				Frag frag;
				frag.x = x;
				frag.y = scanline.y;
				frag.varyings = v0_.varyings + ddx_.varyings * (frag.x - v0_.pos.x) + ddy_.varyings * (frag.y - v0_.pos.y);
				frag.varyings = frag.varyings * ww;
				frag.z = rhw;

				fraglist_.push_back(frag);
			}
			scanline.v = scanline.v + scanline.step;
			if (x >= width_) {
				break;
			}
		}
	}

	void traversalTrapezoid(Trapezoid& trap) {
		Scanline scanline;
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
	Vert ddx_, ddy_;
	Vert v0_;
	std::vector<Frag> fraglist_;
	std::shared_ptr<FragShader> fragshader_;
};


SHAKURAS_END;