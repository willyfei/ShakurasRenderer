#pragma once
#include "MathAndGeometry.h"
#include "GsPrimitive.h"
#include "GsPipeline.h"
#include <vector>


SHAKURAS_BEGIN;


class GsScanline;
class GsTrapezoid;


class GsRasterizerStage : public IGsRasterizerStage {
public:
	virtual void initialize(int ww, int hh, void* fb);
	virtual void process(In& input);

private:
	void clear();
	void renderPrimitive(const GsVertex* tri, GsTextureU32Ptr texture);
	void renderPrimitive(const GsVertex* line);
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