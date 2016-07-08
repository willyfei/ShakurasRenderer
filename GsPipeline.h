#pragma once
#include "GsViewer.h"


SHAKURAS_BEGIN;


template<class Uniform, class Vert>
struct GsStageBuffer {
	Uniform uniform;
	std::vector<Vert> vertlist;
	std::vector<std::array<int, 3> > itris;
};


template<class StageBuffer, class AppStage, class GeomStage, class RasStage>
class GsPipeline {
public:
	void initialize(GsViewerPtr viewer) {
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