#pragma once
#include "MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;


template<class C>
class GsTexture {
public:
	GsTexture() {}

public:
	void reset(const C* data, int ww, int hh) {
		width_ = ww;
		height_ = hh;

		buffer.resize(height_, std::vector<uint32_t>(width_));

		for (int j = 0; j < height_; ++j)
			for (int i = 0; i < width_; ++i)
				buffer[j][i] = data[j*width_ + i];
	}

	C at(float u, float v) {
		u *= (width_ - 1);
		v *= (height_ - 1);
		int x = (int)(u + 0.5f);
		int y = (int)(v + 0.5f);
		x = Clamp(x, 0, width_ - 1);
		y = Clamp(y, 0, height_ - 1);
		return buffer[y][x];
	}

public:
	std::vector<std::vector<C> > buffer;
	int width_, height_;
};


typedef GsTexture<uint32_t> GsTextureU32;
SHAKURAS_SHARED_PTR(GsTextureU32);


SHAKURAS_END;