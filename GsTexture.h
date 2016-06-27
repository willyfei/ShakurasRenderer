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
		w = ww;
		h = hh;

		buffer.resize(h, std::vector<uint32_t>(w));

		for (int j = 0; j < h; ++j)
			for (int i = 0; i < w; ++i)
				buffer[j][i] = data[j*w + i];
	}

	C at(float u, float v) {
		u *= (w - 1);
		v *= (h - 1);
		int x = (int)(u + 0.5f);
		int y = (int)(v + 0.5f);
		x = Restrict(x, 0, w - 1);
		y = Restrict(y, 0, h - 1);
		return buffer[y][x];
	}

public:
	std::vector<std::vector<C> > buffer;
	int w, h;
};


typedef GsTexture<uint32_t> GsTextureU32;
SHAKURAS_SHARED_PTR(GsTextureU32);


SHAKURAS_END;