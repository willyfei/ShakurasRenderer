#pragma once
#include "Viewer.h"


SHAKURAS_BEGIN;


template<class CMD, class AS, class GS, class RS>
class GraphicPipeline {
public:
	void initialize(ViewerPtr viewer) {
		appstage_.initialize(viewer);
		geomstage_.initialize((float)viewer->width(), (float)viewer->height());
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		std::vector<CMD> cmds;
		appstage_.process(cmds);

		rasstage_.clean();

		for (auto i = cmds.begin(); i != cmds.end(); i++) {
			geomstage_.process(*i);
			rasstage_.process(*i);
		}
	}


protected:
	AS appstage_;
	GS geomstage_;
	RS rasstage_;
};


SHAKURAS_END;