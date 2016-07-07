#pragma once
#include "Utility.h"
#include "GsViewer.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


class GsApplicationStage {
public:
	struct Out {
		float width;
		float height;

		Matrix44f viewtrsf;
		Matrix44f projtrsf;

		std::vector<GsVertexA8V4> vertlist;
		std::vector<Matrix44f> modeltrsflist;
		GsTextureU32Ptr texture;

		std::vector<std::array<int, 3> > itris;//vertlist Ë÷Òý
		std::vector<int> itrsfs;//modeltrsflist Ë÷Òý
	};


	virtual void initialize(GsViewerPtr viewer);
	virtual void process(Out& output);

private:
	GsViewerPtr viewer_;
	Out output_;
	float alpha_;
	float pos_;
};


SHAKURAS_END;