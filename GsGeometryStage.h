#pragma once
#include "MathAndGeometry.h"
#include "GsDrawable.h"
#include "GsApplicationStage.h"
#include <vector>


SHAKURAS_BEGIN;


class GsGeometryStage {
public:
	void initialize(float w, float h);

	typedef GsApplicationStage::Out In;
	typedef std::vector<GsDrawable> Out;
	void process(In& input, Out& output);

private:
	float width_, height_;
	Matrix44f viewtrsf_, projtrsf_;
};


SHAKURAS_END;