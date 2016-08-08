// Example_Soft_Cup.cpp : 定义控制台应用程序的入口点。
//

#include "SoftRenderer\SoftPreset.h"
#include "Core\Application.h"
#include "ResourceParser\TextureLoader.h"
#include "PlatformSpec\WinViewer.h"
#include "ResourceParser\ObjParser.h"


using namespace shakuras;


namespace soft_cup {

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
				soft_phong::DrawCall& cmd = outputs_[i];

				cmd.prims.verts_.resize(mesh.verts.size());
				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];
					soft_phong::Vertex& v = cmd.prims.verts_[ii];

					v.pos.set(objv.pos.x, objv.pos.y, objv.pos.z, 1.0f);
					v.attribs.uv = objv.uv;
					v.attribs.normal = objv.normal;
				}

				for (size_t ii = 0; ii < mesh.tris.size(); ii += 3) {
					cmd.prims.tris_.push_back({ mesh.tris[ii], mesh.tris[ii + 1], mesh.tris[ii + 2] });
				}

				cmd.proj_trsf = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 0.5f, 500.0f);//投影变换

				SoftSurfacePtr surface = std::make_shared<SoftSurface>();
				int texw = 0, texh = 0;
				void* bits = LoadTexture(mesh.mtl.tex_full_path, false, texw, texh);
				surface->reset(texw, texh, (uint32_t*)bits);
				cmd.uniforms.texture = CreateSoftMipmap(surface);

				cmd.uniforms.ambient = mesh.mtl.ambient;
				cmd.uniforms.diffuse = mesh.mtl.diffuse;
				cmd.uniforms.specular = mesh.mtl.specular;
			}

			alpha_ = 0.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}

		void process(std::vector<soft_phong::DrawCall>& cmds) {
			if (viewer_->testUserMessage(kUMUp)) pos_ += 0.04f;
			if (viewer_->testUserMessage(kUMDown)) pos_ -= 0.04f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ -= 0.02f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ += 0.02f;

			Vector3f eye(0, -3 - pos_, 1.0f), at(0, 0, 0), up(0, 0, 1);
			Vector3f eye_pos = eye;
			Vector3f light_pos(-100.0f, -100.0f, 100.0f);

			Matrix44f modeltrsf = Matrix44f::Rotate(1.0f, 0.0f, 0.0f, 0.5f * kGSPI) * Matrix44f::Rotate(0.0f, 0.0f, 1.0f, alpha_);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			for (size_t i = 0; i != outputs_.size(); i++) {
				soft_phong::DrawCall& cmd = outputs_[i];
				cmd.uniforms.model_trsf = modeltrsf;//模型变换
				cmd.uniforms.mv_trsf = modeltrsf * viewtrsf;//模型*视图变换
				cmd.uniforms.eye_pos = eye_pos;//相机位置
				cmd.uniforms.light_pos = light_pos;//光源位置
			}

			cmds = outputs_;
		}

	private:
		WinViewerPtr viewer_;
		std::vector<soft_phong::DrawCall> outputs_;
		float alpha_;
		float pos_;
	};

	typedef shakuras::Application<soft_phong::DrawCall, AppStage, soft_phong::RenderStage> Application;
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

	soft_cup::Application app;
	app.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		app.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}

