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

	typedef std::tuple<GsTextureU32Ptr, Matrix44f, Matrix44f, Vector3f> Uniform;
	typedef std::tuple<Vector2f, Vector4f> Attrib;
	typedef GsVertex<Attrib, 5> Vertex;
	typedef GsFragment<5> Fragment;
	typedef GsStageBuffer<Uniform, Vertex> StageBuffer;

	void GenerateCube(std::vector<Vertex>& verts, std::vector<std::array<int, 3> >& itris) {
		static Vertex mesh[8]{
			{ { 1, -1,  1, 1 } },
			{ { -1, -1,  1, 1 } },
			{ { -1,  1,  1, 1 } },
			{ { 1,  1,  1, 1 } },
			{ { 1, -1, -1, 1 } },
			{ { -1, -1, -1, 1 } },
			{ { -1,  1, -1, 1 } },
			{ { 1,  1, -1, 1 } },
		};

		auto draw_plane = [&](int a, int b, int c, int d) {
			Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

			std::get<0>(p1.attrib).set(0, 0);
			std::get<0>(p2.attrib).set(0, 1);
			std::get<0>(p3.attrib).set(1, 1);
			std::get<0>(p4.attrib).set(1, 0);

			Vector4f norm = CrossProduct(p3.pos - p2.pos, p1.pos - p2.pos);
			norm.w = 0.0f;
			std::get<1>(p1.attrib) = norm;
			std::get<1>(p2.attrib) = norm;
			std::get<1>(p3.attrib) = norm;
			std::get<1>(p4.attrib) = norm;

			int index = verts.size();
			verts.push_back(p1);
			verts.push_back(p2);
			verts.push_back(p3);
			itris.push_back({ index, index + 1, index + 2 });

			index = verts.size();
			verts.push_back(p3);
			verts.push_back(p4);
			verts.push_back(p1);
			itris.push_back({ index, index + 1, index + 2 });
		};

		draw_plane(0, 1, 2, 3);
		draw_plane(4, 5, 6, 7);
		draw_plane(0, 4, 5, 1);
		draw_plane(1, 5, 6, 2);
		draw_plane(2, 6, 7, 3);
		draw_plane(3, 7, 4, 0);
	}

	class AppStage {
	public:
		void initialize(GsViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			GenerateCube(output_.vertlist, output_.itris);
			std::get<0>(output_.uniform) = LoadTexture("1.png");
			std::get<1>(output_.uniform) = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 1.0f, 500.0f);
			std::get<3>(output_.uniform).set(1, 1, 1);
			Normalize(std::get<3>(output_.uniform));

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

			v.varying[0] = std::get<0>(v.attrib).x;
			v.varying[1] = std::get<0>(v.attrib).y;

			Vector4f norm = mvtrsf.transform(std::get<1>(v.attrib));
			v.varying[2] = norm.x;
			v.varying[3] = norm.y;
			v.varying[4] = norm.z;
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

