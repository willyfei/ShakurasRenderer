#pragma once
#include "MathAndGeometry.h"
#include "GeometryStage.h"
#include "Vertex.h"
#include "Color.h"
#include "Sampler.h"
#include "Profiler.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


struct Edge {
	Vector3f v1, v2;
};


inline Vector3f Lerp(const Edge& edge, float y) {
	return edge.v1 + (edge.v2 - edge.v1) * ((y - edge.v1.y) / (edge.v2.y - edge.v1.y));
}


struct Trapezoid {
	float top, bottom;
	Edge left, right;
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


//每个像素一个片元，每个片元一个采样点
template<class F>
class TrapTraversalStd {
public:
	void initialize(const Trapezoid& trap, int width, int height, std::vector<std::array<F, 4> >& output) {
		trap_ = &trap;
		width_ = width;
		height_ = height;
		output_ = &output;
	}

	void process() {
		for (int i = (int)trap_->top; i <= (int)(trap_->bottom); i += 2) {
			scan(i);
		}
	}

private:
	struct RangeLine {
		Vector3f left, right;
		int xb, xe;
		bool visible;
	};

	void scan(int y0) {
		float yf0 = y0 + 0.5f;
		float yf1 = y0 + 1.5f;

		RangeLine rl0, rl1;

		rl0.visible = (trap_->top <= yf0 && yf0 <= trap_->bottom);
		rl1.visible = (trap_->top <= yf1 && yf1 <= trap_->bottom);

		rl0.left = Lerp(trap_->left, yf0);
		rl0.right = Lerp(trap_->right, yf0);
		rl1.left = Lerp(trap_->left, yf1);
		rl1.right = Lerp(trap_->right, yf1);

		rl0.xb = (std::max)(0, (int)rl0.left.x);
		rl0.xe = (std::min)(width_ - 1, (int)rl0.right.x);
		rl1.xb = (std::max)(0, (int)rl1.left.x);
		rl1.xe = (std::min)(width_ - 1, (int)rl1.right.x);

		int xb = 0, xe = 0;
		if (rl0.visible && rl1.visible) {
			xb = (std::min)(rl0.xb, rl1.xb);
			xe = (std::min)(rl0.xe, rl1.xe);
		}
		else if (rl0.visible) {
			xb = rl0.xb;
			xe = rl0.xe;
		}
		else if (rl1.visible) {
			xb = rl1.xb;
			xe = rl1.xe;
		}
		else {
			return;
		}

		for (int i = xb; i <= xe; i += 2) {
			float xf0 = i + 0.5f;
			float xf1 = i + 1.5f;

			//2, 3
			//0, 1
			std::array<F, 4> tile;
			fragAssign(xf0, yf0, rl0, tile[0]);
			fragAssign(xf1, yf0, rl0, tile[1]);
			fragAssign(xf0, yf1, rl1, tile[2]);
			fragAssign(xf1, yf1, rl1, tile[3]);

			output_->push_back(tile);
		}
	}

	void fragAssign(float xf, float yf, const RangeLine& rl, F& frag) {
		frag.x = (int)xf;
		frag.y = (int)yf;
		frag.z = Lerp(rl.left.z, rl.right.z, (xf - rl.left.x) / (rl.right.x - rl.left.x));
		frag.weight = (rl.visible && (rl.xb <= frag.x && frag.x <= rl.xe) ? 1.0f : 0.0f);
	}

public:
	const Trapezoid* trap_;
	int width_, height_;
	std::vector<std::array<F, 4> >* output_;
};


template<class UL, class F, class FS>
class TileShader {
public:
	void process(const UL& u, std::array<F, 4>& tile, Profiler* profiler) {
		sampler_.ddx_ = TexCoord(tile[1].varyings) - TexCoord(tile[0].varyings);
		sampler_.ddy_ = TexCoord(tile[2].varyings) - TexCoord(tile[0].varyings);
		for (int i = 0; i != 4; i++) {
			if (0.0f < tile[i].weight) {
				fragshader_.process(u, sampler_, tile[i]);
				profiler->count("Frag-Sharder Excuted", 1);
			}
		}
	}

public:
	Sampler sampler_;
	FS fragshader_;
};


template<class V, class F>
class LerpDerivative {
public:
	void setTriangle(const V& v0, const V& v1, const V& v2) {
		V e01 = v1 - v0;
		V e02 = v2 - v0;

		float area = e02.pos.x * e01.pos.y - e01.pos.x * e02.pos.y;
		float inv_area = 1.0f / area;

		v0_ = v0;
		ddx_ = (e02 * e01.pos.y - e01 * e02.pos.y) * inv_area;
		ddy_ = (e01 * e02.pos.x - e02 * e01.pos.x) * inv_area;
	}

	void lerp(F& frag) const {
		float fx = frag.x + 0.5f;
		float fy = frag.y + 0.5f;
		float rhw = frag.z;
		if (rhw == 0.0f) rhw = 0.000001f;
		frag.varyings = (v0_.varyings + ddx_.varyings * (fx - v0_.pos.x) + ddy_.varyings * (fy - v0_.pos.y)) / rhw;
		frag.z = rhw;
	}

private:
	V v0_, ddy_, ddx_;
};


template<class UL, class V, class F, class FS>
class RasterizerStage {
public:
	void initialize(int ww, int hh, void* fb, Profiler& profiler) {
		width_ = ww;
		height_ = hh;
		profiler_ = &profiler;

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

	void process(DrawCall<UL, V>& call) {
		profiler_->count("Ras-Triangle Count", (int)call.prims.tris_.size());
		
		//triangle setup, 省略

		for (size_t i = 0; i != call.prims.tris_.size(); i++) {
			//triangle traversal
			//fragment shader
			//merging
			const std::array<size_t, 3>& tri = call.prims.tris_[i];

			const V& v1 = call.prims.verts_[tri[0]];
			const V& v2 = call.prims.verts_[tri[1]];
			const V& v3 = call.prims.verts_[tri[2]];
			drawTriangle(call.uniforms, v1, v2, v3);
		}
	}

	void clean() {
		uint32_t cc = IBgr(Vector3f(0.2f, 0.2f, 0.6f));
		for (int y = 0; y < height_; y++) {
			uint32_t *dst = framebuffer_[y];
			for (int x = width_; x > 0; dst++, x--) dst[0] = cc;
		}

		for (int y = 0; y < height_; y++) {
			std::vector<float>& dst = zbuffer_[y];
			std::fill(dst.begin(), dst.end(), 0.0f);
		}
	}

private:
	void drawTriangle(const UL& u, V v0, V v1, V v2) {
		v0.rhwInitialize();
		v1.rhwInitialize();
		v2.rhwInitialize();

		LerpDerivative<V, F> lerpd;
		lerpd.setTriangle(v0, v1, v2);

		std::vector<Trapezoid> traps;
		SpliteTrapezoid(v0, v1, v2, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			drawTrapezoid(u, lerpd, *i);
		}
	}

	/*inline bool isPixelCoord(int x, int y) {
		return 0 <= x && x < width_ && 0 <= y && y < height_;
	}

	void drawScanline(const UL& u, const LerpDerivative<V, F>& lerpd, Scanline22& s22) {
		int x = s22.xbegin();
		int w = s22.xwidth();
		TileShader<UL, F, FS> tileshader;

		for (; w > 0; x += 2, w -= 2) {
			if (0 <= x && x < width_) {
				// 2, 3
				// 0, 1
				std::array<F, 4> tile;
				tile[0] = lerpd.lerp(x, s22.s0.y, s22.s0.v.z);
				tile[0].draw = s22.draw0(x) && isPixelCoord(x, s22.s0.y);
				tile[2] = lerpd.lerp(x, s22.s1.y, s22.s1.v.z);
				tile[2].draw = s22.draw1(x) && isPixelCoord(x, s22.s1.y);

				s22.next();

				tile[1] = lerpd.lerp(x + 1, s22.s0.y, s22.s0.v.z);
				tile[1].draw = s22.draw0(x + 1) && isPixelCoord(x + 1, s22.s0.y);
				tile[3] = lerpd.lerp(x + 1, s22.s1.y, s22.s1.v.z);
				tile[3].draw = s22.draw1(x + 1) && isPixelCoord(x + 1, s22.s1.y);

				s22.next();

				//fragment shader
				profiler_->count("Frag Count", 4);
				tileshader.process(u, tile, profiler_);

				//merging
				for (size_t i = 0; i != 4; i++) {
					const F& frag = tile[i];
					if (frag.draw && frag.z > zbuffer_[frag.y][frag.x]) {
						zbuffer_[frag.y][frag.x] = frag.z;
						framebuffer_[frag.y][frag.x] = IRgba(frag.c);
					}
				}
			}
			else {
				s22.next();
				s22.next();
			}

			if (x >= width_) {
				break;
			}
		}
	}*/

	void drawTrapezoid(const UL& u, const LerpDerivative<V, F>& lerpd, Trapezoid& trap) {
		std::vector<std::array<F, 4> > tiles;

		TrapTraversalStd<F> trav;
		trav.initialize(trap, width_, height_, tiles);
		trav.process();

		TileShader<UL, F, FS> tileshader;
		for (auto i = tiles.begin(); i != tiles.end(); i++) {
			lerpd.lerp((*i)[0]);
			lerpd.lerp((*i)[1]);
			lerpd.lerp((*i)[2]);
			lerpd.lerp((*i)[3]);

			profiler_->count("Frag Count", 4);
			tileshader.process(u, *i, profiler_);

			//merging
			merge(*i);
		}

		/*Scanline22 scanline;
		int j, top, bottom;
		top = (int)(trap.top + 0.5f);
		bottom = (int)(trap.bottom + 0.5f);
		for (j = top; j < bottom; j += 2) {
			if (j >= 0 && j < height_) {
				scanline.initialize(trap, j);
				drawScanline(u, lerpd, scanline);
			}
			if (j >= height_) {
				break;
			}
		}*/
	}

	void merge(const std::array<F, 4>& tile) {
		for (size_t i = 0; i != 4; i++) {
			const F& frag = tile[i];
			if (frag.weight != 0.0f && zbuffer_[frag.y][frag.x] <= frag.z) {
				zbuffer_[frag.y][frag.x] = frag.z;
				framebuffer_[frag.y][frag.x] = IRgba(frag.c);
			}
		}
	}

private:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
	Profiler* profiler_;
};


SHAKURAS_END;