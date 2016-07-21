#pragma once
#include "Viewer.h"


SHAKURAS_BEGIN;


template<class SB, class AS, class GS, class RS>
class GraphicPipeline {
public:
	void initialize(ViewerPtr viewer) {
		appstage_.initialize(viewer);
		geomstage_.initialize((float)viewer->width(), (float)viewer->height());
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		SB buffer;
		appstage_.process(buffer);

		geomstage_.process(buffer);

		rasstage_.process(buffer);
	}


protected:
	AS appstage_;
	GS geomstage_;
	RS rasstage_;
};


SHAKURAS_END;