#pragma once
#include "MathAndGeometry.h"
#include "GsPrimitive.h"
#include "GsDrawable.h"
#include "GsGeometryStage.h"
#include <vector>


SHAKURAS_BEGIN;


class GsScanline;
class GsTrapezoid;


class GsRasterizerStage {
public:
	void initialize(int ww, int hh, void* fb);
	typedef GsGeometryStage::Out In;
	void process(In& input);

private:
	void clear();
	void renderPrimitive(const GsTriangle& tri, GsStatePtr state);
	void renderPrimitive(const GsLine& line, GsStatePtr state);
	void drawPixel(int x, int y, uint32_t c);
	void drawLine(int x1, int y1, int x2, int y2, uint32_t c);
	void drawScanline(GsScanline& scanline, GsStatePtr state);
	void drawTrapezoid(GsTrapezoid& trap, GsStatePtr state);

public:
	std::vector<uint32_t*> framebuffer;
	std::vector<std::vector<float> > zbuffer;
	int width, height;
};


SHAKURAS_END;