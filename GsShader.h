#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"


SHAKURAS_BEGIN;


struct GsUniform {
	GsTextureU32Ptr texture;
};


class GsVertexShader {
public:
	void execute(GsVertex& vert, const GsUniform& uniform) {

	}
};


class GsFragmentShader {
public:
	void execute(GsFragment& frag, const GsUniform& uniform) {

	}
};


SHAKURAS_END;