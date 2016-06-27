// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GsRasterizer.h"
#include "GsTransformer.h"
#include "GsResource.h"
#include "GsViewer.h"
#include <Windows.h>


using namespace shakuras;


int main()
{
	std::vector<GsTriangle> cube;
	GenerateCube(cube);
	GsTextureU32Ptr tex = LoadTexture("1.png");

	GsStatePtr states[3];
	states[0] = std::make_shared<GsState>(false, false, true, tex, GsColor24());
	states[1] = std::make_shared<GsState>(false, true, false, nullptr, GsColor24(0.6, 0.3, 0.1));
	states[2] = std::make_shared<GsState>(true, false, false, nullptr, GsColor24());

	int indicator = 0;
	int kbhit = 0;
	float alpha = 1;
	float pos = 3.5;

	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward, Space: switch state";

	int width = 800, height = 600;
	GsViewerPtr viewer = CreateGsViewer("Windows");
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	GsTransformerPtr trsf = std::make_shared<GsTransformer>();
	trsf->initialize(width, height);
	CameraAtZero(trsf, 3, 0, 0);

	GsRasterizer device;
	device.initialize(width, height, viewer->frameBuffer());

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		device.clear();
		CameraAtZero(trsf, pos, 0, 0);

		if (viewer->testUserMessage(kUMUp)) pos -= 0.01f;
		if (viewer->testUserMessage(kUMDown)) pos += 0.01f;
		if (viewer->testUserMessage(kUMLeft)) alpha += 0.01f;
		if (viewer->testUserMessage(kUMRight)) alpha -= 0.01f;

		if (viewer->testUserMessage(kUMSpace)) {
			if (kbhit == 0) {
				kbhit = 1;
				indicator = (indicator + 1) % 3;
			}
		}
		else {
			kbhit = 0;
		}

		trsf->world = Matrix44f::Rotate(-1, -0.5, 1, alpha);
		trsf->updateWVP();

		for (auto i = cube.begin(); i != cube.end(); i++) {
			device.renderPrimitive(*i, trsf, states[indicator]);
		}

		viewer->update();
		Sleep(1);
	}

	return 0;
}

