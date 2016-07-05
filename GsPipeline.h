#pragma once
#include "MathAndGeometry.h"
#include "GsViewer.h"
#include "GsApplicationStage.h"
#include "GsGeometryStage.h"
#include "GsRasterizerStage.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


class GsPipeline {
public:
	void initialize(GsViewerPtr viewer) {
		appstage_.initialize(viewer);
		geomstage_.initialize((float)viewer->width(), (float)viewer->height());
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		GsApplicationStage::Out appopt;
		appstage_.process(appopt);

		GsGeometryStage::Out geomopt;
		geomstage_.process(appopt, geomopt);

		rasstage_.process(geomopt);
	}


protected:
	GsApplicationStage appstage_;
	GsGeometryStage geomstage_;
	GsRasterizerStage rasstage_;
};


SHAKURAS_END;