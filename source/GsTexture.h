#pragma once
#include "MathAndGeometry.h"
#include "GsColor.h"
#include <vector>


SHAKURAS_BEGIN;


class GsSurface {
public:
	GsSurface() {}

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


SHAKURAS_SHARED_PTR(GsSurface);


template<class Surface>
Vector3f NearestSample(float u, float v, Surface& surface) {
	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	x = Clamp(x, 0, surface.width() - 1);
	y = Clamp(y, 0, surface.height() - 1);

	return surface.get(x, y);
}


template<class Surface>
Vector3f BilinearSample(float u, float v, Surface& surface) {
	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int fu = (int)u, fv = (int)v;
	fu = Clamp(fu, 0, surface.width() - 1);
	fv = Clamp(fv, 0, surface.height() - 1);

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


class GsMipmap {
public:
	GsMipmap() {}

public:
	void reset(int ww, int hh, const uint32_t* data) {
		surfaces_.clear();

		GsSurfacePtr surface = std::make_shared<GsSurface>();
		surface->reset(ww, hh, data);
		surfaces_.push_back(surface);

		ww = (std::max)(1, (surface->width() + 1) / 2);
		hh = (std::max)(1, (surface->height() + 1) / 2);
		while (ww > 1 || hh > 1) {
			GsSurfacePtr next_surface = std::make_shared<GsSurface>();
			next_surface->reset(ww, hh, nullptr);

			for (int x = 0; x != next_surface->width(); x++) {
				float u = ((float)x) / (next_surface->width() - 1);

				for (int y = 0; y != next_surface->height(); y++) {
					float v = ((float)y) / (next_surface->height() - 1);

					Vector3f c = BilinearSample(u, v, *surface);
					next_surface->set(x, y, c);
				}
			}

			surface = next_surface;
			surfaces_.push_back(surface);
		}
	}

	inline int width() const { return surfaces_.empty() ? 0 : surfaces_.back()->width(); }
	inline int height() const { return surfaces_.empty() ? 0 : surfaces_.back()->height(); }
	inline Vector3f get(int x, int y, int l) const { return surfaces_[l]->get(x, y); }
	inline void set(int x, int y, int l, const Vector3f& c) { surfaces_[l]->set(x, y, c); }

private:
	std::vector<GsSurfacePtr> surfaces_;
};


SHAKURAS_SHARED_PTR(GsMipmap);





SHAKURAS_END;