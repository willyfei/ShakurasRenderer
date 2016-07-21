// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "Resource.h"
#include "Viewer.h"
#include "GraphicPipeline.h"
#include "Preset.h"
#include <tuple>
#include <Windows.h>
#include "ObjParser.h"


using namespace shakuras;


namespace example_cube {

	void GenerateCube(std::vector<preset_std::Vertex>& verts) {
		static preset_std::Vertex mesh[8] = {
			{ { -1, -1, -1, 1 } },
			{ { 1, -1, -1, 1 } },
			{ { 1, 1, -1, 1 } },
			{ { -1, 1, -1, 1 } },
			{ { -1, -1, 1, 1 } },
			{ { 1, -1, 1, 1 } },
			{ { 1, 1, 1, 1 } },
			{ { -1, 1, 1, 1 } },
		};

		auto draw_plane = [&](int a, int b, int c, int d) {
			preset_std::Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

			p1.attribs.uv.set(0, 0);
			p2.attribs.uv.set(0, 1);
			p3.attribs.uv.set(1, 1);
			p4.attribs.uv.set(1, 0);

			Vector4f norm = CrossProduct(p3.pos - p2.pos, p1.pos - p2.pos);
			norm.w = 0.0f;
			p1.attribs.normal = norm;
			p2.attribs.normal = norm;
			p3.attribs.normal = norm;
			p4.attribs.normal = norm;

			int index = (int)verts.size();
			verts.push_back(p1);
			verts.back().primf = kPFTriangle;
			verts.push_back(p2);
			verts.push_back(p3);

			index = (int)verts.size();
			verts.push_back(p3);
			verts.back().primf = kPFTriangle;
			verts.push_back(p4);
			verts.push_back(p1);
		};

		draw_plane(0, 3, 2, 1);
		draw_plane(4, 5, 6, 7);
		draw_plane(0, 1, 5, 4);
		draw_plane(1, 2, 6, 5);
		draw_plane(2, 3, 7, 6);
		draw_plane(0, 4, 7, 3);
	}

	class AppStage {
	public:
		void initialize(ViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			texlist_.push_back(CreateMipmap(LoadSurface("1.png")));
			texlist_.push_back(CreateMipmap(LoadSurface("1.jpg")));
			texlist_.push_back(CreateMipmap(LoadSurface("2.png")));
			texlist_.push_back(CreateMipmap(GridSurface()));
			itex_ = 0;
			nspace_ = 0;

			GenerateCube(output_.vertlist);
			output_.projtrsf = Matrix44f::Perspective(kGSPI * 0.6f, w / h, 1.0f, 500.0f);//投影变换
			output_.uniforms.texture = texlist_[itex_];//纹理
			output_.uniforms.ambient.set(0.4f, 0.4f, 0.4f);//环境光
			output_.uniforms.diffuse.set(0.587609f, 0.587609f, 0.587609f);//漫反射
			output_.uniforms.specular.set(0.071744f, 0.071744f, 0.071744f);//镜面反射
			output_.uniforms.light_pos.set(100.0f, 100.0f, -100.0f);//光源位置

			alpha_ = 1.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}


		void process(std::vector<preset_std::DrawCallBuffer>& cmds) {
			if (viewer_->testUserMessage(kUMSpace)) {
				if (++nspace_ == 1) {
					itex_ = (itex_ + 1) % texlist_.size();
				}
			}
			else {
				nspace_ = 0;
			}
			if (viewer_->testUserMessage(kUMUp)) pos_ -= 0.04f;
			if (viewer_->testUserMessage(kUMDown)) pos_ += 0.04f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ += 0.02f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ -= 0.02f;

			Vector3f eye(3 + pos_, 0, 0), at(0, 0, 0), up(0, 0, 1);
			output_.uniforms.texture = texlist_[itex_];//纹理
			output_.uniforms.mvtrsf = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);//模型*视图变换
			output_.uniforms.eye_pos = eye;//相机位置

			cmds.push_back(output_);
		}

	private:
		ViewerPtr viewer_;
		preset_std::DrawCallBuffer output_;
		std::vector<MipmapPtr> texlist_;
		int itex_;
		int nspace_;
		float alpha_;
		float pos_;
	};

	typedef GraphicPipeline<preset_std::DrawCallBuffer, AppStage, preset_std::GeomStage, preset_std::RasStage> Pipeline;
}


namespace example_cup {

	class AppStage {
	public:
		void initialize(ViewerPtr viewer) {
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
		ViewerPtr viewer_;
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
	ViewerPtr viewer = CreateViewer("Windows");
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

