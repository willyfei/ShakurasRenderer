#pragma once
#include "MathAndGeometry.h"
#include "GsColor.h"
#include <vector>


SHAKURAS_BEGIN;


class GsTextureSurface {
public:
	GsTextureSurface() {}

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


SHAKURAS_SHARED_PTR(GsTextureSurface);


template<class SurfacePtr>
Vector3f NearestSample(float u, float v, SurfacePtr surface) {
	u *= (surface->width() - 1);
	v *= (surface->height() - 1);

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	x = Clamp(x, 0, surface->width() - 1);
	y = Clamp(y, 0, surface->height() - 1);

	return surface->get(x, y);
}


template<class SurfacePtr>
Vector3f BilinearSample(float u, float v, SurfacePtr surface) {
	u *= (surface->width() - 1);
	v *= (surface->height() - 1);

	int fu = (int)u, fv = (int)v;
	fu = Clamp(fu, 0, surface->width() - 1);
	fv = Clamp(fv, 0, surface->height() - 1);

	int cu = fu + 1, cv = fv + 1;
	cu = Clamp(cu, 0, surface->width() - 1);
	cv = Clamp(cv, 0, surface->height() - 1);

	Vector3f lbc = surface->get(fu, fv);//左下
	Vector3f rbc = surface->get(cu, fv);//右下
	Vector3f ltc = surface->get(fu, cv);//左上
	Vector3f rtc = surface->get(cu, cv);//右上

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