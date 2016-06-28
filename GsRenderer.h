#pragma once
#include "MathAndGeometry.h"
#include "GsViewer.h"
#include "GsApplicationStage.h"
#include "GsGeometryStage.h"
#include "GsRasterizerStage.h"


SHAKURAS_BEGIN;


class GsRenderer {
public:
	void initialize(GsViewerPtr viewer);

	void frame();

private:
	GsApplicationStage appstage;
	GsGeometryStage geomstage;
	GsRasterizerStage rasstage;
};


SHAKURAS_END;