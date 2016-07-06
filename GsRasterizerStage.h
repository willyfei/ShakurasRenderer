#pragma once
#include "MathAndGeometry.h"
#include "GsGeometryStage.h"
#include "GsVertexFragment.h"
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
	void traversalTriangle(const GsVertex& v1, const GsVertex& v2, const GsVertex& v3);
	void traversalScanline(GsScanline& scanline);
	void traversalTrapezoid(GsTrapezoid& trap);

public:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
	std::vector<std::array<int, 2> > fragvalid_;
	std::vector<std::vector<GsFragment> > fragbuffer_;
};


SHAKURAS_END;