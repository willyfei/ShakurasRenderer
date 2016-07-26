#pragma once
#include "MathAndGeometry.h"
#include "Color.h"
#include <vector>
#include <math.h>


SHAKURAS_BEGIN;


inline void ClampAddr(float u, float v, int w, int h, int& x, int& y) {
	u *= (w - 1);
	v *= (h - 1);

	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);

	x = Clamp(x, 0, w - 1);
	y = Clamp(y, 0, h - 1);
}

inline void RepeatAddr(float u, float v, int w, int h, int& x, int& y) {
	u = fmodf(u, 1.0f);
	v = fmodf(v, 1.0f);

	u = (u < 0.0f ? 1.0f + u : u);
	v = (v < 0.0f ? 1.0f + v : v);

	ClampAddr(u, v, w, h, x, y);
}


class Surface {
public:
	Surface() {}

public:
	void reset(int ww, int hh, const uint32_t* data) {
		width_ = ww;
		height_ = hh;

		data_.clear();
		data_.resize(height_, std::vector<uint32_t>(width_, 0));

		if (data) {
			for (int j = 0; j < height_; ++j) {
				for (int i = 0; i < width_; ++i) {
					data_[j][i] = data[j*width_ + i];
				}
			}
		}
	}

	inline int width() const { return width_; }
	inline int height() const { return height_; }
	inline Vector3f get(int x, int y) const { return VRgb(data_[y][x]); }
	inline void set(int x, int y, const Vector3f& c) { data_[y][x] = IRgb(c); }

private:
	std::vector<std::vector<uint32_t> > data_;
	int width_, height_;
};


SHAKURAS_SHARED_PTR(Surface);


template<class S, typename AF>
Vector3f NearestSample(float u, float v, const S& surface, AF addressing) {
	int x = 0, y = 0;
	addressing(u, v, surface.width(), surface.height(), x, y);
	return surface.get(x, y);
}


template<class S, typename AF>
Vector3f BilinearSample(float u, float v, const S& surface, AF addressing) {
	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int fu = 0, fv = 0;
	addressing((float)u, (float)v, surface.width(), surface.height(), fu, fv);

	int cu = fu + 1, cv = fv + 1;
	cu = Clamp(cu, 0, surface.width() - 1);
	cv = Clamp(cv, 0, surface.height() - 1);

	Vector3f lbc = surface.get(fu, fv);//左下
	Vector3f rbc = surface.get(cu, fv);//右下
	Vector3f ltc = surface.get(fu, cv);//左上
	Vector3f rtc = surface.get(cu, cv);//右上

	//u方向插值
	float it = (cu != fu ? (u - fu) / (cu - fu) : 0.0f);
	Vector3f ibc = lbc + (rbc - lbc) * it;
	Vector3f itc = ltc + (rtc - ltc) * it;

	//v方向插值
	it = (cv != fv ? (v - fv) / (cv - fv) : 0.0f);
	Vector3f ic = ibc + (itc - ibc) * it;

	return ic;
}


SHAKURAS_END;