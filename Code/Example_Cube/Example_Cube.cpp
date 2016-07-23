// Example_Cube.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <vector>
#include "Core\Preset.h"
#include "ResourceParser\SurfaceParser.h"
#include "PlatformSpec\WinViewer.h"


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
		void initialize(WinViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			texlist_.push_back(CreateMipmap(LoadSurface("1.png")));
			texlist_.push_back(CreateMipmap(LoadSurface("1.jpg")));
			texlist_.push_back(CreateMipmap(LoadSurface("2.png")));
			texlist_.push_back(CreateMipmap(GridSurface()));
			itex_ = 0;
			nspace_ = 0;

			GenerateCube(output_.vertlist);
			output_.projtrsf = Matrix44f::Perspective(kGSPI * 0.6f, w / h, 1.0f, 500.0f);//ͶӰ�任
			output_.uniforms.texture = texlist_[itex_];//����
			output_.uniforms.ambient.set(0.4f, 0.4f, 0.4f);//������
			output_.uniforms.diffuse.set(0.587609f, 0.587609f, 0.587609f);//������
			output_.uniforms.specular.set(0.071744f, 0.071744f, 0.071744f);//���淴��
			output_.uniforms.light_pos.set(100.0f, 100.0f, -100.0f);//��Դλ��

			alpha_ = 1.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}

		void process(std::vector<preset_std::DrawCommand>& cmds) {
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
			output_.uniforms.texture = texlist_[itex_];//����
			output_.uniforms.mvtrsf = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);//ģ��*��ͼ�任
			output_.uniforms.eye_pos = eye;//���λ��

			cmds.push_back(output_);
		}

	private:
		WinViewerPtr viewer_;
		preset_std::DrawCommand output_;
		std::vector<MipmapPtr> texlist_;
		int itex_;
		int nspace_;
		float alpha_;
		float pos_;
	};

	typedef GraphicPipeline<preset_std::DrawCommand, AppStage, preset_std::GeomStage, preset_std::RasStage> Pipeline;
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

	example_cube::Pipeline pipeline;
	pipeline.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		pipeline.process();

		viewer->update();
		Sleep(1);
	}

    return 0;
}
