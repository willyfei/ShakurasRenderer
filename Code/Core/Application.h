#pragma once
#include "Profiler.h"


SHAKURAS_BEGIN;


template<class CALL, class AS, class RS>
class Application {
public:
	template<class VPTR>
	bool initialize(VPTR viewer) {
		bool app_ok = appstage_.initialize(viewer);
		renstage_.initialize(viewer, profiler_);
		return app_ok;
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