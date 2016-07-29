// Example_Sponza.cpp : 定义控制台应用程序的入口点。
//

#include "Core\Preset.h"
#include "ResourceParser\SurfaceParser.h"
#include "PlatformSpec\WinViewer.h"
#include "ResourceParser\ObjParser.h"


using namespace shakuras;


namespace example_sponza {

	struct UniformList {
		MipmapPtr texture;
		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
		Matrix44f mvp_trsf;
		Vector3f light_pos;
		Vector3f eye_pos;
	};

	class VertexShader {
	public:
		void process(const UniformList& u, preset_std::Vertex& v) {
			v.varyings.normal = v.attribs.normal;

			v.pos = u.mvp_trsf.transform(v.pos);

			v.varyings.light_dir = u.light_pos - v.pos.xyz();

			v.varyings.uv = v.attribs.uv;
		}
	};


	class FragmentShader {
	public:
		void process(const UniformList& u, Sampler& sampler, preset_std::Fragment& f) {
			Vector3f norm = f.varyings.normal;
			Vector3f light_dir = f.varyings.light_dir;
			Normalize3(norm);
			Normalize3(light_dir);

			Vector2f uv = f.varyings.uv;
			Vector3f diff_color(1.0f, 1.0f, 1.0f);//默认白色
			if (u.texture) {
				diff_color = sampler.surfaceBilinear(uv.x, uv.y, u.texture->level(0), RepeatAddr);
			}

			float illum_diffuse = Clamp(DotProduct3(light_dir, norm), 0.0f, 1.0f);
			Vector3f c = diff_color * illum_diffuse;

			Clamp(c.x, 0.0f, 1.0f);
			Clamp(c.y, 0.0f, 1.0f);
			Clamp(c.z, 0.0f, 1.0f);

			f.c.set(c.x, c.y, c.z, 1.0f);
		}
	};

	typedef shakuras::DrawCall<UniformList, preset_std::Vertex> DrawCall;

	typedef shakuras::GeometryStage<UniformList, preset_std::Vertex, VertexShader> GeomStage;

	typedef shakuras::RasterizerStage<UniformList, preset_std::Vertex, preset_std::Fragment, FragmentShader> RasStage;

	class AppStage {
	public:
		void initialize(WinViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			std::vector<ObjMesh> meshs;
			LoadObjMesh("Sponza/part_of_sponza.obj", meshs, false);

			outputs_.clear();
			outputs_.resize(meshs.size());

			for (size_t i = 0; i != meshs.size(); i++) {
				const ObjMesh& mesh = meshs[i];
				DrawCall& cmd = outputs_[i];

				cmd.prims.verts_.resize(mesh.verts.size());
				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];
					preset_std::Vertex& v = cmd.prims.verts_[ii];

					v.pos.set(objv.pos.x, objv.pos.y, objv.pos.z, 1.0f);
					v.attribs.uv = objv.uv;
					v.attribs.normal = objv.normal;
				}

				for (size_t ii = 0; ii < mesh.tris.size(); ii += 3) {
					cmd.prims.tris_.push_back({ mesh.tris[ii], mesh.tris[ii + 1], mesh.tris[ii + 2] });
				}

				projtrsf_ = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 0.1f, 1000.0f);//投影变换
				cmd.proj_trsf.reset();
				cmd.uniforms.texture = mesh.mtl.tex;
				cmd.uniforms.ambient = mesh.mtl.ambient;
				cmd.uniforms.diffuse = mesh.mtl.diffuse;
				cmd.uniforms.specular = mesh.mtl.specular;
			}

			step_ = 0;

			viewer_ = viewer;
		}

		void process(std::vector<DrawCall>& cmds) {
			if (viewer_->testUserMessage(kUMUp)) step_++;
			if (viewer_->testUserMessage(kUMDown)) step_--;

			float xpos = -36.0f + fmodf(0.625f * step_, 66.0f);
			float ypos = 10.0f + fmodf(0.5f * step_, 40.0f);

			Vector3f eye(xpos, 8.0f, 0.0f), at(40.0f, 15.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
			Vector3f eye_pos = eye;
			Vector3f light_pos(0.0f, ypos, 0.0f);

			Matrix44f modeltrsf = Matrix44f::Translate(-0.5f, 0.0f, -0.5f);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			for (size_t i = 0; i != outputs_.size(); i++) {
				DrawCall& cmd = outputs_[i];

				cmd.uniforms.mvp_trsf = modeltrsf * viewtrsf * projtrsf_;//模型*视图*投影
				cmd.uniforms.eye_pos = eye_pos;//相机位置
				cmd.uniforms.light_pos = light_pos;//光源位置
			}

			cmds = outputs_;
		}

	private:
		WinViewerPtr viewer_;
		Matrix44f projtrsf_;
		std::vector<DrawCall> outputs_;
		int step_;
	};

	typedef shakuras::GraphicPipeline<DrawCall, AppStage, GeomStage, RasStage> Pipeline;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 512, height = 512;
	WinViewerPtr viewer = std::make_shared<WinViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	example_sponza::Pipeline pipeline;
	pipeline.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		pipeline.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}


