#include "stdafx.h"
#include "GsApplicationStage.h"
#include "GsResource.h"



SHAKURAS_BEGIN;


void GsApplicationStage::initialize(GsViewerPtr viewer) {
	output_.width = (float)viewer->width();
	output_.height = (float)viewer->height();
	output_.projtrsf = Matrix44f::Perspective(kGSPI * 0.5f, output_.width / output_.height, 1.0f, 500.0f);

	output_.texturelist.push_back(LoadTexture("1.png"));
	GenerateCube(output_.vertlist, output_.itris);
	output_.itexs.resize(output_.itris.size(), 0);

	alpha_ = 1;
	pos_ = 3.5;

	viewer_ = viewer;
}

void GsApplicationStage::process(Out& output) {
	if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.02f;
	if (viewer_->testUserMessage(kUMDown)) pos_ += 0.02f;
	if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.01f;
	if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.01f;

	Vector3f eye = { 3 + pos_, 0, 0 }, at = { 0, 0, 0 }, up = { 0, 0, 1 };

	output = output_;
	output.viewtrsf = Matrix44f::LookAt(eye, at, up);
	output.modeltrsflist.push_back(Matrix44f::Rotate(-1, -0.5, 1, alpha_));
	output.itrsfs.resize(output_.itris.size(), 0);
}


SHAKURAS_END;