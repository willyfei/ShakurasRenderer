// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GsResource.h"
#include "GsViewer.h"
#include <Windows.h>
#include "GsPipeline.h"
#include "GsApplicationStage.h"
#include "GsGeometryStage.h"
#include "GsRasterizerStage.h"


using namespace shakuras;


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 1600, height = 900;
	GsViewerPtr viewer = CreateGsViewer("Windows");
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	GsPipelineSpec<GsApplicationStage, GsGeometryStage, GsRasterizerStage> pipeline;
	pipeline.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		pipeline.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}

