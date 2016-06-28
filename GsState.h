#pragma once
#include "MathAndGeometry.h"
#include "GsTexture.h"
#include "GsPrimitive.h"


SHAKURAS_BEGIN;


class GsState {
public:
	GsState() : drawwireframe(true), drawcolor(false), drawtexture(false), wirecolor(1.0f, 1.0f, 1.0f) {}
	GsState(bool dwf, bool dc, bool dt, GsTextureU32Ptr t, GsColor24 c)
		: drawwireframe(dwf), drawcolor(dc), drawtexture(dt), texture(t), wirecolor(c) {}

public:
	bool drawwireframe;
	bool drawcolor;
	bool drawtexture;
	GsTextureU32Ptr texture;
	GsColor24 wirecolor;
};


SHAKURAS_SHARED_PTR(GsState);


SHAKURAS_END;