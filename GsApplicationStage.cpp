#include "stdafx.h"
#include "GsApplicationStage.h"
#include "GsResource.h"



SHAKURAS_BEGIN;


void GsApplicationStage::initialize(GsViewerPtr viewer) {
	texture_ = LoadTexture("1.png");
	GenerateCube(cube_);

	alpha_ = 1;
	pos_ = 3.5;

	viewer_ = viewer;
}

void GsApplicationStage::process(Out& output) {
	if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.02f;
	if (viewer_->testUserMessage(kUMDown)) pos_ += 0.02f;
	if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.01f;
	if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.01f;

	Vector4f eye = { 3 + pos_, 0, 0, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	output.viewtrsf = Matrix44f::LookAt(eye, at, up);
	output.objects.push_back({ Matrix44f::Rotate(-1, -0.5, 1, alpha_), texture_, cube_ });
}


SHAKURAS_END;