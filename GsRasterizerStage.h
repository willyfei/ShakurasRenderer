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
	void traversalTriangle(const std::array<GsVertex, 3>& tri, int ti);
	void traversalScanline(GsScanline& scanline, int ti);
	void traversalTrapezoid(GsTrapezoid& trap, int ti);

public:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
	std::vector<std::vector<std::vector<GsFragment> > > fragbuffer_;
};


SHAKURAS_END;