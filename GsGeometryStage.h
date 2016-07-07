#pragma once
#include "MathAndGeometry.h"
#include "GsApplicationStage.h"


SHAKURAS_BEGIN;


class GsGeometryStage {
public:
	typedef GsApplicationStage::Out In;
	struct Out {
		std::vector<GsVertexA4> vertlist;
		GsTextureU32Ptr texture;

		std::vector<std::array<int, 3> > itris;//vertlist Ë÷Òý
	};


	virtual void initialize(float w, float h);
	virtual void process(In& input, Out& output);

private:
	float width_, height_;
};


SHAKURAS_END;