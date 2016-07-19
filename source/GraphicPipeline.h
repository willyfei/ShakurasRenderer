#pragma once
#include "Viewer.h"


SHAKURAS_BEGIN;


template<class StageBuffer, class AppStage, class GeomStage, class RasStage>
class GraphicPipeline {
public:
	void initialize(ViewerPtr viewer) {
		appstage_.initialize(viewer);
		geomstage_.initialize((float)viewer->width(), (float)viewer->height());
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		StageBuffer buffer;
		appstage_.process(buffer);

		geomstage_.process(buffer);

		rasstage_.process(buffer);
	}


protected:
	AppStage appstage_;
	GeomStage geomstage_;
	RasStage rasstage_;
};


SHAKURAS_END;