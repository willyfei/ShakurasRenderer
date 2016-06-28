#include "stdafx.h"
#include "GsApplicationStage.h"
#include "GsResource.h"



SHAKURAS_BEGIN;


void GsApplicationStage::initialize(GsViewerPtr viewer) {
	GsTextureU32Ptr texture = LoadTexture("1.png");
	GenerateCube(cube_);
	states_[0] = std::make_shared<GsState>(false, false, true, texture, GsColor24());
	states_[1] = std::make_shared<GsState>(false, true, false, nullptr, GsColor24(0.6f, 0.3f, 0.1f));
	states_[2] = std::make_shared<GsState>(true, false, false, nullptr, GsColor24());

	indicator_ = 0;
	kbhit_ = 0;
	alpha_ = 1;
	pos_ = 3.5;

	viewer_ = viewer;
}

void GsApplicationStage::process(Out& output) {
	if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.02f;
	if (viewer_->testUserMessage(kUMDown)) pos_ += 0.02f;
	if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.01f;
	if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.01f;

	if (viewer_->testUserMessage(kUMSpace)) {
		if (kbhit_ == 0) {
			kbhit_ = 1;
			indicator_ = (indicator_ + 1) % 3;
		}
	}
	else {
		kbhit_ = 0;
	}

	Vector4f eye = { 3 + pos_, 0, 0, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	output.viewtrsf = Matrix44f::LookAt(eye, at, up);
	output.drawables.push_back({ Matrix44f::Rotate(-1, -0.5, 1, alpha_), states_[indicator_], cube_ });
}


SHAKURAS_END;