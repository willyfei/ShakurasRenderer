#pragma once
#include "Utility.h"
#include "GsViewer.h"
#include "GsPipeline.h"


SHAKURAS_BEGIN;


class GsApplicationStage : public IGsApplicationStage {
public:
	virtual void initialize(GsViewerPtr viewer);
	virtual void process(Out& output);

private:
	GsViewerPtr viewer_;
	GsTextureU32Ptr texture_;
	std::vector<GsVertex> cube_;
	float alpha_;
	float pos_;
};


SHAKURAS_END;