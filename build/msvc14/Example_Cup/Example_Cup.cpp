// Example_Cup.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include "Core\Preset.h"
#include "ResourceParser\SurfaceParser.h"
#include "PlatformSpec\WinViewer.h"
#include "ResourceParser\ObjParser.h"


using namespace shakuras;


namespace example_cup {

	class AppStage {
	public:
		void initialize(WinViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			std::vector<ObjMesh> meshs;
			LoadObjMesh("cup.obj", meshs, false);

			outputs_.clear();
			outputs_.resize(meshs.size());

			for (size_t i = 0; i != meshs.size(); i++) {
				const ObjMesh& mesh = meshs[i];
				preset_std::DrawCallBuffer& dcb = outputs_[i];

				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];
					preset_std::Vertex v;

					v.pos.set(objv.pos.x, objv.pos.y, objv.pos.z, 1.0f);
					v.attribs.uv = objv.uv;
					v.attribs.normal.set(objv.normal.x, objv.normal.y, objv.normal.z, 0.0f);
					if (ii % 3 == 0) {
						v.primf = kPFTriangle;
					}

					dcb.vertlist.push_back(v);
				}

				dcb.projtrsf = Matrix44f::Perspective(kGSPI * 0.6f, w / h, 1.0f, 500.0f);//投影变换
				dcb.uniforms.texture = mesh.mtl.tex;
				dcb.uniforms.ambient = mesh.mtl.ambient;
				dcb.uniforms.diffuse = mesh.mtl.diffuse;
				dcb.uniforms.specular = mesh.mtl.specular;
				dcb.uniforms.light_pos.set(100.0f, 100.0f, -100.0f);//光源位置
			}

			alpha_ = 1.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}


		void process(std::vector<preset_std::DrawCallBuffer>& cmds) {
			if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.04f;
			if (viewer_->testUserMessage(kUMDown)) pos_ += 0.04f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.02f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.02f;

			Vector3f eye(3 + pos_, 0, 0), at(0, 0, 0), up(0, 0, 1);
			Matrix44f mvtrsf = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);

			for (size_t i = 0; i != outputs_.size(); i++) {
				preset_std::DrawCallBuffer& dcb = outputs_[i];
				dcb.uniforms.mvtrsf = mvtrsf;//模型*视图变换
				dcb.uniforms.eye_pos = eye;//相机位置
			}

			cmds = outputs_;
		}

	private:
		WinViewerPtr viewer_;
		std::vector<preset_std::DrawCallBuffer> outputs_;
		float alpha_;
		float pos_;
	};

	typedef GraphicPipeline<preset_std::DrawCallBuffer, AppStage, preset_std::GeomStage, preset_std::RasStage> Pipeline;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward, Space: switch texture";

	int width = 1600, height = 900;
	WinViewerPtr viewer = std::make_shared<WinViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	example_cup::Pipeline pipeline;
	pipeline.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		pipeline.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}

