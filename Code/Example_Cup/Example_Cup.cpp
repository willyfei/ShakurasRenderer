// Example_Cup.cpp : 定义控制台应用程序的入口点。
//

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
			LoadObjMesh("Cup/cup.obj", meshs, false);

			outputs_.clear();
			outputs_.resize(meshs.size());

			for (size_t i = 0; i != meshs.size(); i++) {
				const ObjMesh& mesh = meshs[i];
				preset_std::DrawCall& cmd = outputs_[i];

				cmd.prims.verts_.resize(mesh.verts.size());
				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];
					preset_std::Vertex& v = cmd.prims.verts_[ii];

					v.pos.set(objv.pos.x, objv.pos.y, objv.pos.z, 1.0f);
					v.attribs.uv = objv.uv;
					v.attribs.normal.set(objv.normal.x, objv.normal.y, objv.normal.z, 0.0f);
				}

				for (size_t ii = 0; ii < mesh.tris.size(); ii += 3) {
					cmd.prims.tris_.push_back({ mesh.tris[ii], mesh.tris[ii + 1], mesh.tris[ii + 2] });
				}

				cmd.projtrsf = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 0.5f, 500.0f);//投影变换
				cmd.uniforms.texture = mesh.mtl.tex;
				cmd.uniforms.ambient = mesh.mtl.ambient;
				cmd.uniforms.diffuse = mesh.mtl.diffuse;
				cmd.uniforms.specular = mesh.mtl.specular;
				cmd.uniforms.light_pos.set(100.0f, 100.0f, -100.0f);//光源位置
			}

			alpha_ = 1.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}

		void process(std::vector<preset_std::DrawCall>& cmds) {
			if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.04f;
			if (viewer_->testUserMessage(kUMDown)) pos_ += 0.04f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.02f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.02f;

			Vector3f eye(3 + pos_, 0, 0), at(0, 0, 0), up(0, 0, 1);
			Vector4f light_pos(100.0f, -100.0f, 100.0f, 1.0f);

			Matrix44f modeltrsf = Matrix44f::Rotate(-1, -0.5, 1, alpha_);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);
			Matrix44f mvtrsf = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);
			light_pos = viewtrsf.transform(light_pos);

			for (size_t i = 0; i != outputs_.size(); i++) {
				preset_std::DrawCall& cmd = outputs_[i];
				cmd.uniforms.mvtrsf = mvtrsf;//模型*视图变换
				cmd.uniforms.eye_pos = eye;//相机位置
				cmd.uniforms.light_pos.set(light_pos.x, light_pos.y, light_pos.z);//光源位置
			}

			cmds = outputs_;
		}

	private:
		WinViewerPtr viewer_;
		std::vector<preset_std::DrawCall> outputs_;
		float alpha_;
		float pos_;
	};

	typedef GraphicPipeline<preset_std::DrawCall, AppStage, preset_std::GeomStage, preset_std::RasStage> Pipeline;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 1024, height = 768;
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

