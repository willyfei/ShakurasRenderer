#include "stdafx.h"
#include "GsRenderer.h"


SHAKURAS_BEGIN;


void GsRenderer::initialize(GsViewerPtr viewer) {
	appstage.initialize(viewer);
	geomstage.initialize((float)viewer->width(), (float)viewer->height());
	rasstage.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
}

void GsRenderer::frame() {
	GsApplicationStage::Out appopt;
	appstage.process(appopt);

	GsGeometryStage::Out geomopt;
	geomstage.process(appopt, geomopt);

	rasstage.process(geomopt);
}


SHAKURAS_END;