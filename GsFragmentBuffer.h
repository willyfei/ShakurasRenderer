#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"


SHAKURAS_BEGIN;


class GsFragmentBuffer {
public:
	void reset(const GsVertex& v1, const GsVertex& v2, const GsVertex& v3) {
		float xmin = (std::min)((std::min)(v1.pos.x, v2.pos.x), v3.pos.x);
		float xmax = (std::max)((std::max)(v1.pos.x, v2.pos.x), v3.pos.x);
		float ymin = (std::min)((std::min)(v1.pos.y, v2.pos.y), v3.pos.y);
		float ymax = (std::max)((std::max)(v1.pos.y, v2.pos.y), v3.pos.y);

		xbegin_ = (int)floorf(xmin - 0.5f);
		ybegin_ = (int)floorf(ymin - 0.5f);
		xend_ = (int)roundf(xmax + 0.5f) + 1;
		yend_ = (int)roundf(ymax + 0.5f) + 1;

		xbegin_ = (std::max)(xbegin_, 0);
		ybegin_ = (std::max)(ybegin_, 0);

		int ylen = yend_ - ybegin_;
		int xlen = xend_ - xbegin_;

		fragvalid_.clear();
		fragbuffer_.clear();
		fragvalid_.resize(ylen * xlen, false);
		fragbuffer_.resize(ylen * xlen);
	}

	const GsFragment& fragmentAt(int x, int y) const {
		int yy = y - ybegin_;
		int xx = x - xbegin_;
		return fragbuffer_[yy * (xend_ - xbegin_) + xx];
	}

	GsFragment& fragmentAt(int x, int y) {
		int yy = y - ybegin_;
		int xx = x - xbegin_;
		return fragbuffer_[yy * (xend_ - xbegin_) + xx];
	}

	bool fragmentValid(int x, int y) {
		int yy = y - ybegin_;
		int xx = x - xbegin_;
		return fragvalid_[yy * (xend_ - xbegin_) + xx];
	}

	void travelsal(int x, int y, const GsFragment& frag) {
		int yy = y - ybegin_;
		int xx = x - xbegin_;
		fragvalid_[yy * (xend_ - xbegin_) + xx] = true;
		fragbuffer_[yy * (xend_ - xbegin_) + xx] = frag;
	}

	template<class S>
	void shader(S& sampler) {
		for (int y = ybegin_; y != yend_; y++) {
			for (int x = xbegin_; x != xend_; x++) {
				if (fragmentValid(x, y)) {
					GsFragment& frag = fragmentAt(x, y);
					frag.c = sampler.at(frag.tc.u, frag.tc.v);
				}
			}
		}
	}

	template<class FB, class ZB>
	void merge(FB& framebuffer, ZB& zbuffer) {
		for (int y = ybegin_; y != yend_; y++) {
			for (int x = xbegin_; x != xend_; x++) {
				const GsFragment& frag = fragmentAt(x, y);
				if (frag.z > zbuffer[y][x] && fragmentValid(x, y)) {
					zbuffer[y][x] = frag.z;
					framebuffer[y][x] = frag.c;
				}
			}
		}
	}

private:
	int xbegin_, ybegin_;
	int xend_, yend_;
	std::vector<bool> fragvalid_;
	std::vector<GsFragment> fragbuffer_;
};


SHAKURAS_END;