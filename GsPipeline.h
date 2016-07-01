#pragma once
#include "MathAndGeometry.h"
#include "GsViewer.h"
#include "GsTexture.h"
#include "GsPrimitive.h"
#include <vector>


SHAKURAS_BEGIN;



struct IGsApplicationStage{
	struct Object {
		Matrix44f modeltrsf;
		GsTextureU32Ptr texture;
		std::vector<GsVertex> trianglelist;
	};


	struct Out {
		std::vector<Object> objects;
		Matrix44f viewtrsf;
	};


	virtual void initialize(GsViewerPtr viewer) = 0;
	virtual void process(Out& output) = 0;
};


struct IGsGeometryStage {
	struct Geometric {
		GsTextureU32Ptr texture;
		std::vector<GsVertex> trianglelist;
	};


	typedef IGsApplicationStage::Out In;
	typedef std::vector<Geometric> Out;


	virtual void initialize(float w, float h) = 0;
	virtual void process(In& input, Out& output) = 0;
};


struct IGsRasterizerStage {
	typedef IGsGeometryStage::Out In;


	virtual void initialize(int ww, int hh, void* fb) = 0;
	virtual void process(In& input) = 0;
};



class GsPipeline {
public:
	void initialize(GsViewerPtr viewer) {
		appstage_->initialize(viewer);
		geomstage_->initialize((float)viewer->width(), (float)viewer->height());
		rasstage_->initialize(viewer->width(), viewer->height(), viewer->frameBuffer());
	}

	void process() {
		IGsApplicationStage::Out appopt;
		appstage_->process(appopt);

		IGsGeometryStage::Out geomopt;
		geomstage_->process(appopt, geomopt);

		rasstage_->process(geomopt);
	}


protected:
	std::shared_ptr<IGsApplicationStage> appstage_;
	std::shared_ptr<IGsGeometryStage> geomstage_;
	std::shared_ptr<IGsRasterizerStage> rasstage_;
};


template<class AppStage, class GeomStage, class RasStage>
class GsPipelineSpec : public GsPipeline
{
public:
	GsPipelineSpec() {
		appstage_ = std::make_shared<AppStage>();
		geomstage_ = std::make_shared<GeomStage>();
		rasstage_ = std::make_shared<RasStage>();
	}
};


SHAKURAS_END;