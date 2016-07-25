#pragma once


SHAKURAS_BEGIN;


template<class CALL, class AS, class GS, class RS>
class GraphicPipeline {
public:
	template<class VPTR>
	void initialize(VPTR viewer) {
		appstage_.initialize(viewer);
		geomstage_.initialize((float)viewer->width(), (float)viewer->height());
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		std::vector<CALL> calls;
		appstage_.process(calls);

		rasstage_.clean();

		for (auto i = calls.begin(); i != calls.end(); i++) {
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