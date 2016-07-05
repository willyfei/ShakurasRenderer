#pragma once
#include "MathAndGeometry.h"
#include "GsGeometryStage.h"
#include "GsVertexFragment.h"
#include "GsFragmentBuffer.h"
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
	void traversalTriangle(const GsVertex& v1, const GsVertex& v2, const GsVertex& v3, GsFragmentBuffer& fragbuf);
	void traversalScanline(GsScanline& scanline, GsFragmentBuffer& fragbuf);
	void traversalTrapezoid(GsTrapezoid& trap, GsFragmentBuffer& fragbuf);

public:
	std::vector<uint32_t*> framebuffer_;
	std::vector<std::vector<float> > zbuffer_;
	int width_, height_;
};


SHAKURAS_END;