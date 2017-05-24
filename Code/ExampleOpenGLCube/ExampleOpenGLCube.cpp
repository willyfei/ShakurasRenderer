// Example_OpenGL_Cube.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "OpenGLRenderer\GlPreset.h"
#include "OpenGLRenderer\GlBatch.h"
#include "OpenGLRenderer\GlDrawCall.h"
#include "OpenGLRenderer\GlRenderStage.h"
#include "Core\Application.h"
#include "ResourceParser\TextureLoader.h"
#include "PlatformSpec\WinViewer.h"
#include "OpenGLRenderer\GlContextBinding.h"


using namespace shakuras;


namespace opengl_cube {

	GlMipmapPtr LoadMipmap(std::string tex_full_path) {
		int texw = 0, texh = 0;
		void* bits = LoadTexture(tex_full_path, true, texw, texh);
		GlMipmapPtr mipmap = CreateGlMipmap(bits, texw, texh);
		ResFree(bits);
		return mipmap;
	}

	GlMipmapPtr GridMipmap() {
		int texw = 0, texh = 0;
		void* bits = GridTexture(texw, texh);
		GlMipmapPtr mipmap = CreateGlMipmap(bits, texw, texh);
		ResFree(bits);
		return mipmap;
	}

	GlVAOPtr GenerateCube() {
		static const float mesh_val = 1.0f;
		static Vector3f mesh[8] = {
			{ -mesh_val, -mesh_val, -mesh_val },
			{ mesh_val, -mesh_val, -mesh_val },
			{ mesh_val, mesh_val, -mesh_val },
			{ -mesh_val, mesh_val, -mesh_val },
			{ -mesh_val, -mesh_val, mesh_val },
			{ mesh_val, -mesh_val, mesh_val },
			{ mesh_val, mesh_val, mesh_val },
			{ -mesh_val, mesh_val, mesh_val },
		};

		GlVAOFactory factory;

		factory.setPrimtiveCat(GlVAO::kTriangles);

		factory.registerAttrib(GlPhongProgram::kVertPos, 3);
		factory.registerAttrib(GlPhongProgram::kVertNormal, 3);
		factory.registerAttrib(GlPhongProgram::kVertUV, 2);

		auto draw_plane = [&](GlVAOFactory& factory, int a, int b, int c, int d) {
			Vector3f p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
			Vector3f norm = CrossProduct3(p3 - p2, p1 - p2);
			Normalize3(norm);

			unsigned int vert_index = factory.addVertex();
			factory.setAttrib3fv(GlPhongProgram::kVertPos, p1.data());
			factory.setAttrib3fv(GlPhongProgram::kVertNormal, norm.data());
			factory.setAttrib2f(GlPhongProgram::kVertUV, 0, 0);

			factory.addVertex();
			factory.setAttrib3fv(GlPhongProgram::kVertPos, p2.data());
			factory.setAttrib3fv(GlPhongProgram::kVertNormal, norm.data());
			factory.setAttrib2f(GlPhongProgram::kVertUV, 0, 1);

			factory.addVertex();
			factory.setAttrib3fv(GlPhongProgram::kVertPos, p3.data());
			factory.setAttrib3fv(GlPhongProgram::kVertNormal, norm.data());
			factory.setAttrib2f(GlPhongProgram::kVertUV, 1, 1);

			factory.addVertex();
			factory.setAttrib3fv(GlPhongProgram::kVertPos, p4.data());
			factory.setAttrib3fv(GlPhongProgram::kVertNormal, norm.data());
			factory.setAttrib2f(GlPhongProgram::kVertUV, 1, 0);

			factory.addIndex(vert_index);
			factory.addIndex(vert_index + 1);
			factory.addIndex(vert_index + 2);

			factory.addIndex(vert_index + 2);
			factory.addIndex(vert_index + 3);
			factory.addIndex(vert_index);
		};

		draw_plane(factory, 0, 3, 2, 1);
		draw_plane(factory, 4, 5, 6, 7);
		draw_plane(factory, 0, 1, 5, 4);
		draw_plane(factory, 1, 2, 6, 5);
		draw_plane(factory, 2, 3, 7, 6);
		draw_plane(factory, 0, 4, 7, 3);

		return factory.createVAO();
	}

	class AppStage {
	public:
		bool initialize(WinRcViewerPtr viewer) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
			GlContextBinding contex(viewer->hdc(), viewer->hrc());
#elif defined(__ANDROID__)
			GlContextBinding contex(app_);
#endif

			program_ = std::make_shared<GlPhongProgram>();
			if (!program_->initialize()) {
				return false;
			}

			batch_ = GenerateCube();

			texlist_.push_back(LoadMipmap("Cube/1.png"));
			texlist_.push_back(LoadMipmap("Cube/1.jpg"));
			texlist_.push_back(LoadMipmap("Cube/2.png"));
			texlist_.push_back(GridMipmap());
			itex_ = 0;
			nspace_ = 0;

			float aspect = (float)viewer->width() / (float)viewer->height();
			proj_trsf_ = Matrix44f::Perspective(kGSPI * 0.6f, aspect, 1.0f, 500.0f);//ͶӰ�任

			setUniforms();

			alpha_ = 0.0f;
			pos_ = 3.5f;

			viewer_ = viewer;

			return true;
		}

		void process(std::vector<GlDrawCall>& cmds) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
			GlContextBinding contex(viewer_->hdc(), viewer_->hrc());
#elif defined(__ANDROID__)
			GlContextBinding contex(app_);
#endif
			

			if (viewer_->testUserMessage(kUMSpace)) {
				if (++nspace_ == 1) {
					itex_ = (itex_ + 1) % texlist_.size();
				}
			}
			else {
				nspace_ = 0;
			}
			if (viewer_->testUserMessage(kUMUp)) pos_ += 0.004f;
			if (viewer_->testUserMessage(kUMDown)) pos_ -= 0.004f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ -= 0.002f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ += 0.002f;

			setUniforms();

			GlDrawCall drawcall;
			drawcall.batchs.push_back(batch_);
			drawcall.program = program_;

			cmds.push_back(drawcall);
		}

		void setUniforms() {
			program_->use();

			Vector3f eye(0, -3 - pos_, 2.0f), at(0, 0, 0), up(0, 0, 1);
			Vector3f eye_pos = eye;
			Vector3f light_pos(-100.0f, -100.0f, 100.0f);

			Matrix44f modeltrsf = Matrix44f::Rotate(0.0f, 0.0f, 1.0f, alpha_);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			program_->setAmbient({ 0.4f, 0.4f, 0.4f });
			program_->setDiffuse({ 0.587609f, 0.587609f, 0.587609f });
			program_->setSpecular({ 0.071744f, 0.071744f, 0.071744f });
			program_->setModelTrsf(modeltrsf);
			program_->setMvpTrsf(modeltrsf * viewtrsf * proj_trsf_);
			program_->setEyePos(eye_pos);
			program_->setLightPos(light_pos);
			program_->setTexture(texlist_[itex_]);
		}

	private:
		WinRcViewerPtr viewer_;
		GlVAOPtr batch_;
		GlPhongProgramPtr program_;
		Matrix44f proj_trsf_;
		std::vector<GlMipmapPtr> texlist_;
		int itex_;
		int nspace_;
		float alpha_;
		float pos_;
		android_app* app_;
	};

	typedef shakuras::Application<GlDrawCall, AppStage, GlRenderStage> Application;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward, Space: switch texture";

	int width = 1024, height = 768;
	WinRcViewerPtr viewer = std::make_shared<WinRcViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	opengl_cube::Application app;
	if (!app.initialize(viewer)) {
		return -1;
	}

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		app.process();

		viewer->update();
		//Sleep(1);
		sleep(1);
	}

	return 0;
}