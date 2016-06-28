#pragma once
#include "Utility.h"
#include "GsViewer.h"
#include "GsDrawable.h"
#include <array>


SHAKURAS_BEGIN;


class GsApplicationStage {
public:
	void initialize(GsViewerPtr viewer);

	struct Out {
		std::vector<GsDrawable> drawables;
		Matrix44f viewtrsf;
	};
	void process(Out& output);

private:
	GsViewerPtr viewer_;
	std::vector<GsTriangle> cube_;
	std::array<GsStatePtr, 3> states_;
	int indicator_;
	int kbhit_;
	float alpha_;
	float pos_;
};


SHAKURAS_END;