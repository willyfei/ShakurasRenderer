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

		int ww = (std::max)(1, (surface->width() + 1) / 2);
		int hh = (std::max)(1, (surface->height() + 1) / 2);
		while (ww > 1 || hh > 1) {
			SurfacePtr next_surface = std::make_shared<Surface>();
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

			ww = (std::max)(1, (surface->width() + 1) / 2);
			hh = (std::max)(1, (surface->height() + 1) / 2);
		}
	}

	inline int levelCount() const { return (int)surfaces_.size(); }
	inline const Surface& level(int l) const  { return *surfaces_[l]; }

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
Vector3f NearestSample(float u, float v, float du, float dv, const M& mipmap) {
	int ilow = 0, ihigh = 0;
	float dlow = 0.0f, dhigh = 0.0f;
	for (int i = 0; i != mipmap.levelCount(); i++) {
		float d = (std::max)(du * mipmap.level(i).width(), dv * mipmap.level(i).height());
		if (d < 1.0f) {
			ihigh = i;
			dhigh = d;
			break;
		}
		ilow = i;
		dlow = d;
	}

	return NearestSample(u, v, mipmap.level(ihigh));
}


template<class M>
Vector3f BilinearSample(float u, float v, float du, float dv, const M& mipmap) {
	int ilow = 0, ihigh = 0;
	float dlow = 0.0f, dhigh = 0.0f;
	for (int i = 0; i != mipmap.levelCount(); i++) {
		float d = (std::max)(du * mipmap.level(i).width(), dv * mipmap.level(i).height());
		if (d < 1.0f) {
			ihigh = i;
			dhigh = d;
			break;
		}
		ilow = i;
		dlow = d;
	}

	return BilinearSample(u, v, mipmap.level(ihigh));
}


template<class M>
Vector3f TrilinearSample(float u, float v, float du, float dv, const M& mipmap) {
	int ilow = 0, ihigh = 0;
	float dlow = 0.0f, dhigh = 0.0f;
	for (int i = 0; i != mipmap.levelCount(); i++) {
		float d = (std::max)(du * mipmap.level(i).width(), dv * mipmap.level(i).height());
		if (d < 1.0f) {
			ihigh = i;
			dhigh = d;
			break;
		}
		ilow = i;
		dlow = d;
	}

	Vector3f clow = BilinearSample(u, v, mipmap.level(ilow));
	if (ilow == ihigh) {
		return clow;
	}

	Vector3f chigh = BilinearSample(u, v, mipmap.level(ihigh));

	float t = (1.0f - dlow) / (dhigh - dlow);
	return clow + (chigh - clow) * t;
}


SHAKURAS_END;