#pragma once
#include "MathAndGeometry.h"
#include "GsPipeline.h"


SHAKURAS_BEGIN;


class GsGeometryStage : public IGsGeometryStage {
public:
	virtual void initialize(float w, float h);
	virtual void process(In& input, Out& output);

private:
	float width_, height_;
	Matrix44f viewtrsf_, projtrsf_;
};


SHAKURAS_END;