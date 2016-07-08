// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GsResource.h"
#include "GsViewer.h"
#include <Windows.h>
#include "GsPipeline.h"
#include "GsGeometryStage.h"
#include "GsRasterizerStage.h"
#include <tuple>


using namespace shakuras;


namespace skexample {

	typedef std::tuple<GsTextureU32Ptr, Matrix44f, Matrix44f> Uniform;
	typedef GsVertex<Vector2f, 2> Vertex;
	typedef GsFragment<2> Fragment;
	typedef GsStageBuffer<Uniform, Vertex> StageBuffer;

	class AppStage {
	public:
		void initialize(GsViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			GenerateCube(output_.vertlist, output_.itris);
			std::get<0>(output_.uniform) = LoadTexture("1.png");
			std::get<1>(output_.uniform) = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 1.0f, 500.0f);

			alpha_ = 1;
			pos_ = 3.5;

			viewer_ = viewer;
		}


		void process(StageBuffer& buffer) {
			if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.02f;
			if (viewer_->testUserMessage(kUMDown)) pos_ += 0.02f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.01f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.01f;

			buffer = output_;

			Vector3f eye = { 3 + pos_, 0, 0 }, at = { 0, 0, 0 }, up = { 0, 0, 1 };
			std::get<2>(output_.uniform) = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);
		}

	private:
		GsViewerPtr viewer_;
		StageBuffer output_;
		float alpha_;
		float pos_;
	};


	class VertexShader {
	public:
		void process(const Uniform& u, Vertex& v) {
			const Matrix44f& mvtrsf = std::get<2>(u);
			v.pos = mvtrsf.transform(v.pos);
			v.varying[0] = v.attrib.x;
			v.varying[1] = v.attrib.y;
		}
	};


	class FragmentShader {
	public:
		void process(const Uniform& u, Fragment& f) {
			f.c = std::get<0>(u)->at(f.varying[0], f.varying[1]);
		}
	};


	typedef GsGeometryStage<Uniform, Vertex, VertexShader> GeomStage;
	typedef GsRasterizerStage<Uniform, Vertex, Fragment, FragmentShader> RasStage;
	typedef GsPipeline<StageBuffer, AppStage, GeomStage, RasStage> Pipeline;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 1600, height = 900;
	GsViewerPtr viewer = CreateGsViewer("Windows");
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	skexample::Pipeline pipeline;
	pipeline.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		pipeline.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}

