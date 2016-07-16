// Shakuras.cpp : 定义控制台应用程序的入口点。
//

#include "GsResource.h"
#include "GsViewer.h"
#include <Windows.h>
#include "GsStageBuffer.h"
#include "GsPipeline.h"
#include "GsGeometryStage.h"
#include "GsRasterizerStage.h"
#include <tuple>


using namespace shakuras;


namespace skexample {

	typedef std::tuple<
		GsTextureSurfacePtr,//纹理
		Matrix44f,//投影变换
		Matrix44f,//模型*视图变换
		Vector3f,//环境光颜色
		Vector3f,//漫反射颜色
		Vector3f,//镜面反射颜色
		Vector3f,//光源位置
		Vector3f//相机位置
	> Uniform;

	typedef std::tuple<
		Vector2f,//纹理坐标
		Vector4f//法向
	> Attrib;

	typedef GsVarying<
		Vector2f, kTBAll,//纹理坐标
		Vector3f, kTBAll,//法向
		Vector3f, kTBAll,//光源方向(light_pos - pos)
		Vector3f, kTBAll//相机方向(eye_pos - pos)
	> Varying;

	typedef GsVertex<Attrib, Varying> Vertex;

	typedef GsFragment<Varying> Fragment;

	typedef GsStageBuffer<Uniform, Vertex> StageBuffer;

	void GenerateCube(std::vector<Vertex>& verts, std::vector<int>& itris) {
		static Vertex mesh[8] = {
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

			int index = (int)verts.size();
			verts.push_back(p1);
			verts.push_back(p2);
			verts.push_back(p3);
			itris.push_back(index);
			itris.push_back(index + 1);
			itris.push_back(index + 2);

			index = (int)verts.size();
			verts.push_back(p3);
			verts.push_back(p4);
			verts.push_back(p1);
			itris.push_back(index);
			itris.push_back(index + 1);
			itris.push_back(index + 2);
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
		void initialize(GsViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			texlist_.push_back(LoadTexture("1.png"));
			texlist_.push_back(LoadTexture("2.png"));
			texlist_.push_back(LoadTexture("1.jpg"));
			itex_ = 0;
			nspace_ = 0;

			GenerateCube(output_.vertlist, output_.itris);
			std::get<0>(output_.uniform) = texlist_[itex_];//纹理
			std::get<1>(output_.uniform) = Matrix44f::Perspective(kGSPI * 0.6f, w / h, 1.0f, 500.0f);//投影变换
			std::get<3>(output_.uniform).set(0.4f, 0.4f, 0.4f);//环境光
			std::get<4>(output_.uniform).set(0.587609f, 0.587609f, 0.587609f);//漫反射
			std::get<5>(output_.uniform).set(0.071744f, 0.071744f, 0.071744f);//镜面反射
			std::get<6>(output_.uniform).set(100.0f, 0.0f, 0.0f);//光源位置

			alpha_ = 1.0f;
			pos_ = 3.5f;

			viewer_ = viewer;
		}


		void process(StageBuffer& buffer) {
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

			buffer = output_;

			Vector3f eye(3 + pos_, 0, 0), at(0, 0, 0), up(0, 0, 1);
			std::get<0>(output_.uniform) = texlist_[itex_];//纹理
			std::get<2>(output_.uniform) = Matrix44f::Rotate(-1, -0.5, 1, alpha_) * Matrix44f::LookAt(eye, at, up);//模型*视图变换
			std::get<7>(output_.uniform) = eye;//相机位置
		}

	private:
		GsViewerPtr viewer_;
		StageBuffer output_;
		std::vector<GsTextureSurfacePtr> texlist_;
		int itex_;
		int nspace_;
		float alpha_;
		float pos_;
	};

	class VertexShader {
	public:
		void process(const Uniform& u, Vertex& v) {
			const Matrix44f& mvtrsf = std::get<2>(u);

			v.varying.value0 = std::get<0>(v.attrib);

			Vector4f norm = mvtrsf.transform(std::get<1>(v.attrib));
			v.varying.value1.set(norm.x, norm.y, norm.z);

			Vector3f light_pos = std::get<6>(u);
			v.varying.value2.set(light_pos.x - v.pos.x, light_pos.y - v.pos.y, light_pos.z - v.pos.z);

			Vector3f eye_pos = std::get<7>(u);
			v.varying.value3.set(eye_pos.x - v.pos.x, eye_pos.y - v.pos.y, eye_pos.z - v.pos.z);

			v.pos = mvtrsf.transform(v.pos);
		}
	};

	class FragmentShader {
	public:
		void process(const Uniform& u, Fragment& f) {
			Vector3f norm = f.varying.value1;
			Vector3f light_dir = f.varying.value2;
			Vector3f eye_dir = f.varying.value3;
			Normalize(norm);
			Normalize(light_dir);
			Normalize(eye_dir);

			Vector3f ambient = std::get<3>(u);
			Vector3f diffuse = std::get<4>(u);
			Vector3f specular = std::get<5>(u);

			float illum_diffuse = Clamp(DotProduct(light_dir, norm), 0.0f, 1.0f);
			float illum_specular = Clamp(DotProduct(Reflect(light_dir, norm), eye_dir), 0.0f, 1.0f);
			Vector3f illum = ambient + diffuse * illum_diffuse + specular * illum_specular;

			Vector2f uv = f.varying.value0;
			Vector3f tc = BilinearSample(uv.x, uv.y, *std::get<0>(u));
			Vector3f c(tc.x * illum.x, tc.y * illum.y, tc.z * illum.z);

			Clamp(c.x, 0.0f, 1.0f);
			Clamp(c.y, 0.0f, 1.0f);
			Clamp(c.z, 0.0f, 1.0f);

			f.c.set(c.x, c.y, c.z, 1.0f);
		}
	};

	typedef GsGeometryStage<Uniform, Vertex, VertexShader> GeomStage;
	typedef GsRasterizerStage<Uniform, Vertex, Fragment, FragmentShader> RasStage;
	typedef GsPipeline<StageBuffer, AppStage, GeomStage, RasStage> Pipeline;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward, Space: switch texture";

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

