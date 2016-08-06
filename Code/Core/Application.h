#pragma once
#include "Profiler.h"


SHAKURAS_BEGIN;


template<class CALL, class AS, class RS>
class Application {
public:
	template<class VPTR>
	void initialize(VPTR viewer) {
		appstage_.initialize(viewer);
		renstage_.initialize(viewer, profiler_);
	}

	void process() {
		ScopeProfiling prof(profiler_);

		std::vector<CALL> calls;

		appstage_.process(calls);

		renstage_.process(calls);
	}

public:
	Profiler profiler_;
	AS appstage_;
	RS renstage_;
};


SHAKURAS_END;