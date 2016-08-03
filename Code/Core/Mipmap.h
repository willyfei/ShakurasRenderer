#pragma once
#include "Surface.h"
#include <vector>


SHAKURAS_BEGIN;


class Mipmap {
public:
	Mipmap() {}

public:
	void reset(SurfacePtr surface) {
		surfaces_.clear();

		if (!surface) {
			return;
		}

		surfaces_.push_back(surface);

		int w = (surface->width() + 1) / 2;
		int h = (surface->height() + 1) / 2;
		while (w > 1 || h > 1) {
			SurfacePtr next_surface = std::make_shared<Surface>();
			next_surface->reset(w, h, nullptr);

			for (int x = 0; x != w; x++) {
				int xx = x * 2;

				for (int y = 0; y != h; y++) {
					int yy = y * 2;

					Vector3f c0 = surface->get(xx, yy);
					Vector3f c1 = surface->get((xx + 1) % surface->width(), yy);
					Vector3f c2 = surface->get(xx, (yy + 1) % surface->height());
					Vector3f c3 = surface->get((xx + 1) % surface->width(), (yy + 1) % surface->height());

					next_surface->set(x, y, (c0 + c1 + c2  + c3) * 0.25f);
				}
			}

			surface = next_surface;
			surfaces_.push_back(surface);

			w = (w + 1) / 2;
			h = (h + 1) / 2;
		}
	}

	inline int levelCount() const { return (int)surfaces_.size(); }
	inline const Surface& level(int l) const  {
		if (l < 0) {
			return *surfaces_.front();
		}
		else if (levelCount() <= l) {
			return *surfaces_.back();
		}
		return *surfaces_[l];
	}

private:
	std::vector<SurfacePtr> surfaces_;
};


SHAKURAS_SHARED_PTR(Mipmap);


inline MipmapPtr CreateMipmap(SurfacePtr surface) {
	if (!surface) { 
		return nullptr;
	}

	MipmapPtr mipmap = std::make_shared<Mipmap>();
	mipmap->reset(surface);

	return mipmap;
}


template<class M>
float ComputeLevel(const Vector2f& ddx, const Vector2f& ddy, const M& mipmap) {
	int w = mipmap.level(0).width();
	int h = mipmap.level(0).height();

	Vector2f ddx_ts(ddx.x * w, ddx.y * h);
	Vector2f ddy_ts(ddy.x * w, ddy.y * h);

	float ddx_rho = Length2(ddx_ts);
	float ddy_rho = Length2(ddy_ts);

	float rho = (std::max)(ddx_rho, ddy_rho);

	if (rho == 0.0f) {
		rho = 0.000001f;
	}
	float lambda = log2(rho);
	return lambda - 1.0f;
}


template<class M, typename AF>
Vector3f NearestSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const M& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	float lvf = floorf(lv);
	lvf = Clamp(lvf, 0.0f, (float)mipmap.levelCount() - 1);
	return NearestSample(u, v, mipmap.level((int)lvf), addressing);
}


template<class M, typename AF>
Vector3f BilinearSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const M& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	float lvf = floorf(lv);

	lvf = Clamp(lvf, 0.0f, (float)mipmap.levelCount() - 1);
	return BilinearSample(u, v, mipmap.level((int)lvf), addressing);
}


template<class M, typename AF>
Vector3f TrilinearSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const M& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	int lvf = (int)lv;
	int lvc = lvf + 1;

	lv = Clamp(lv, 0.0f, (float)mipmap.levelCount() - 1);
	lvf = Clamp(lvf, 0, mipmap.levelCount() - 1);
	lvc = Clamp(lvc, 0, mipmap.levelCount() - 1);

	if (lvf == lvc) {
		return BilinearSample(u, v, mipmap.level(lvf), addressing);;
	}

	Vector3f cf = BilinearSample(u, v, mipmap.level(lvf), addressing);
	Vector3f cc = BilinearSample(u, v, mipmap.level(lvc), addressing);

	float t = (lv - lvf) / (lvc - lvf);
	return cf + (cc - cf) * t;
}


SHAKURAS_END;