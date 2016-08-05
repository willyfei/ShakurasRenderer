#pragma once
#include "../Profiler.h"


SHAKURAS_BEGIN;


template<class CALL, class GS, class RS>
class SoftRenderStage {
public:
	template<class VPTR>
	void initialize(VPTR viewer, Profiler& profiler) {
		geostage_.initialize((float)viewer->width(), (float)viewer->height(), profiler);
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer(), profiler);
	}

	void clean() {
		rasstage_.clean();
	}

	void process(CALL& call) {
		geostage_.process(call);
		rasstage_.process(call);
	}

public:
	GS geostage_;
	RS rasstage_;
};


SHAKURAS_END;