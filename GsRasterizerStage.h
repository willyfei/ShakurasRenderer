#pragma once
#include "MathAndGeometry.h"
#include "GsGeometryStage.h"
#include "GsVertex.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


class GsScanline;
class GsTrapezoid;


class GsRasterizerStage {
public:
	typedef GsGeometryStage::Out In;


	virtual void initialize(int ww, int hh, void* fb);
	virtual void process(In& input);

private:
	void clear();
	void renderPrimitive(const GsVertex& v1, const GsVertex& v2, const GsVertex& v3, GsTextureU32Ptr texture);
	void renderPrimitive(const GsVertex& v1, const GsVertex& v2);
	void drawPixel(int x, int y, uint32_t c);
	void drawLine(int x1, int y1, int x2, int y2, uint32_t c);
	void drawScanline(GsScanline& scanline, GsTextureU32Ptr texture);
	void drawTrapezoid(GsTrapezoid& trap, GsTextureU32Ptr texture);

public:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
};


SHAKURAS_END;