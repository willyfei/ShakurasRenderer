// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GsRasterizerStage.h"
#include "GsResource.h"
#include "GsViewer.h"
#include <Windows.h>
#include "GsRenderer.h"


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

	int width = 1600, height = 900;
	GsViewerPtr viewer = CreateGsViewer("Windows");
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	GsRenderer renderer;
	renderer.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		renderer.frame();

		viewer->update();
		Sleep(1);
	}

	return 0;
}

