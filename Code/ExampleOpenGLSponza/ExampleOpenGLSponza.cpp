// ExampleOpenGLSponza.cpp : 定义控制台应用程序的入口点。
//


#include <vector>
#include "OpenGLRenderer\GlPreset.h"
#include "OpenGLRenderer\GlBatch.h"
#include "OpenGLRenderer\GlDrawCall.h"
#include "OpenGLRenderer\GlRenderStage.h"
#include "OpenGLRenderer\GlContextBinding.h"
#include "Core\Application.h"
#include "ResourceParser\TextureLoader.h"
#include "ResourceParser\ObjParser.h"
#include "PlatformSpec\WinViewer.h"


using namespace shakuras;


namespace opengl_sponza {

	GlMipmapPtr LoadMipmap(std::string tex_full_path) {
		int texw = 0, texh = 0;
		void* bits = LoadTexture(tex_full_path, false, texw, texh);
		GlMipmapPtr mipmap = CreateGlMipmap(bits, texw, texh);
		ResFree(bits);
		return mipmap;
	}


	class SponzaProgram : public GlProgram {
	public:
		virtual ~SponzaProgram() {}

	public:
		bool initialize() {
			std::vector<std::string> attrib_locs;
			attrib_locs.push_back("vertPos");
			attrib_locs.push_back("vertNormal");
			attrib_locs.push_back("vertUV");
			return initSharder(vertexSharderSource(), fragmentSharderSource(), attrib_locs);
		}


	public:
		enum AttribIndex {
			kVertPos = 0, kVertNormal = 1, kVertUV = 2, kAttribCount = 3
		};

		//uniforms
		void setAmbient(const Vector3f& ambient) {
			setUniform3fv("ambient", ambient.data());
		}
		void setDiffuse(const Vector3f& diffuse) {
			setUniform3fv("diffuse", diffuse.data());
		}
		void setSpecular(const Vector3f& specular) {
			setUniform3fv("specular", specular.data());
		}
		void setMvpTrsf(const Matrix44f& mvp_trsf) {
			setUniformMatrix4fv("mvpTrsf", mvp_trsf.flat().data());
		}
		void setLightPos(const Vector3f& light_pos) {
			setUniform3fv("lightPos", light_pos.data());
		}
		void setEyePos(const Vector3f& eye_pos) {
			setUniform3fv("eyePos", eye_pos.data());
		}
		void setTexture(GlMipmapPtr tex) {
			setUniformTexture2D("colorMap", tex);
		}

	private:
		const char* vertexSharderSource() {
			static const char * vs_src =
				"uniform mat4 mvpTrsf;"
				"uniform vec3 lightPos;"
				"uniform vec3 eyePos;"
				"attribute vec4 vertPos;"
				"attribute vec3 vertNormal;"
				"attribute vec2 vertUV;"
				"varying vec3 varyingNormal;"
				"varying vec2 varyingUV;"
				"varying vec3 varyingLightDir;"
				"varying vec3 varyingEyeDir;"
				"void main(void) "
				"{"
				"	varyingNormal = vertNormal;"
				"	varyingUV = vertUV;"
				"	varyingLightDir = lightPos - vertPos.xyz;"
				"	varyingEyeDir = eyePos - vertPos.xyz;"
				"	gl_Position = mvpTrsf * vertPos;"
				"}";

			return vs_src;
		}
		const char* fragmentSharderSource() {
			static const char * fs_src =
				"uniform vec3 ambient;"
				"uniform vec3 diffuse;"
				"uniform vec3 specular;"
				"uniform sampler2D colorMap;"
				"in vec3 varyingNormal;"
				"in vec2 varyingUV;"
				"in vec3 varyingLightDir;"
				"in vec3 varyingEyeDir;"
				"void main(void) "
				"{"
				"	vec3 norm = normalize(varyingNormal);"
				"	vec3 lightDir = normalize(varyingLightDir);"
				"	vec3 eyeDir = normalize(varyingEyeDir);"
				""
				"	vec4 texColor = texture(colorMap, varyingUV.st);"
				"	float illum = clamp(dot(lightDir, norm), 0.0, 1.0);"
				""
				"	float r = clamp(texColor.x * illum, 0.0, 1.0);"
				"	float g = clamp(texColor.y * illum, 0.0, 1.0);"
				"	float b = clamp(texColor.z * illum, 0.0, 1.0);"
				""
				"	gl_FragColor = vec4(r, g, b, 1.0);"
				"}";

			return fs_src;
		}
	};

	typedef std::shared_ptr<SponzaProgram> SponzaProgramPtr;


	class SponzaProgramShifter : public GlAbsProgram {
	public:
		virtual ~SponzaProgramShifter() {}

	public:
		virtual void use() {
			if (program_) {
				program_->use();
			}
		}

		virtual void prepare() {
			if (program_) {
				program_->use();
				program_->setAmbient(ambient_);
				program_->setDiffuse(diffuse_);
				program_->setSpecular(specular_);
				program_->setMvpTrsf(mvp_trsf_);
				program_->setLightPos(light_pos_);
				program_->setEyePos(eye_pos_);
				program_->setTexture(tex_);
			}
		}

	public:
		//uniforms
		Vector3f ambient_;
		Vector3f diffuse_;
		Vector3f specular_;
		Matrix44f mvp_trsf_;
		Vector3f light_pos_;
		Vector3f eye_pos_;
		GlMipmapPtr tex_;

		SponzaProgramPtr program_;
	};

	typedef std::shared_ptr<SponzaProgramShifter> SponzaProgramShifterPtr;

	class AppStage {
	public:
		bool initialize(WinRcViewerPtr viewer) {
			GlContextBinding binding(viewer->hdc(), viewer->hrc());

			program_ = std::make_shared<SponzaProgram>();
			if (!program_->initialize()) {
				return false;
			}

			std::vector<ObjMesh> meshs;
			LoadObjMesh("Sponza/sponza.obj", meshs, false);

			batchs_.clear();
			shifters_.clear();

			for (size_t i = 0; i != meshs.size(); i++) {
				const ObjMesh& mesh = meshs[i];
	
				GlVAOFactory factory;

				factory.setPrimtiveCat(GlVAO::kTriangles);

				factory.registerAttrib(SponzaProgram::kVertPos, 3);
				factory.registerAttrib(SponzaProgram::kVertNormal, 3);
				factory.registerAttrib(SponzaProgram::kVertUV, 2);

				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];

					factory.addVertex();

					factory.setAttrib3fv(SponzaProgram::kVertPos, objv.pos.data());
					factory.setAttrib3fv(SponzaProgram::kVertNormal, objv.normal.data());
					factory.setAttrib2fv(SponzaProgram::kVertUV, objv.uv.data());
				}

				for (size_t ii = 0; ii < mesh.tris.size(); ii += 3) {
					factory.addIndex((unsigned int)mesh.tris[ii]);
					factory.addIndex((unsigned int)mesh.tris[ii + 1]);
					factory.addIndex((unsigned int)mesh.tris[ii + 2]);
				}

				GlVAOPtr batch = factory.createVAO();

				SponzaProgramShifterPtr shifter = std::make_shared<SponzaProgramShifter>();
				shifter->program_ = program_;

				shifter->tex_ = LoadMipmap(mesh.mtl.tex_full_path);
				shifter->ambient_ = mesh.mtl.ambient;
				shifter->diffuse_ = mesh.mtl.diffuse;
				shifter->specular_ = mesh.mtl.specular;

				batchs_.push_back(batch);
				shifters_.push_back(shifter);
			}

			float aspect = (float)viewer->width()/ (float)viewer->height();
			projtrsf_ = Matrix44f::Perspective(kGSPI * 0.5f, aspect, 5.0f, 1000.0f);//投影变换
			step_ = 0;
			move_ = 0;

			viewer_ = viewer;

			return true;
		}

		void process(std::vector<GlDrawCall>& cmds) {
			GlContextBinding binding(viewer_->hdc(), viewer_->hrc());

			if (viewer_->testUserMessage(kUMUp)) step_++;
			if (viewer_->testUserMessage(kUMDown)) step_--;
			if (viewer_->testUserMessage(kUMLeft)) step_++;
			if (viewer_->testUserMessage(kUMRight)) step_--;

			float xpos = -30.0f + fmodf(0.00625f * step_, 66.0f);
			float ypos = 10.0f + fmodf(0.005f * step_, 40.0f);

			Vector3f eye(xpos, 12.5f, 0.0f), at(40.0f, 15.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
			Vector3f eye_pos = eye;
			Vector3f light_pos(0.0f, ypos, 0.0f);

			Matrix44f modeltrsf = Matrix44f::Translate(-0.5f, 0.0f, -0.5f);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			for (size_t i = 0; i != shifters_.size(); i++) {
				SponzaProgramShifterPtr shifter = shifters_[i];

				shifter->mvp_trsf_ = modeltrsf * viewtrsf * projtrsf_;//模型*视图*投影
				shifter->eye_pos_ = eye_pos;//相机位置
				shifter->light_pos_ = light_pos;//光源位置

				GlDrawCall call;
				call.program = shifter;
				call.batchs.push_back(batchs_[i]);

				cmds.push_back(call);
			}
		}

	private:
		WinRcViewerPtr viewer_;
		Matrix44f projtrsf_;
		SponzaProgramPtr program_;
		std::vector<GlBatchPtr> batchs_;
		std::vector<SponzaProgramShifterPtr> shifters_;
		int step_, move_;
	};

	typedef shakuras::Application<GlDrawCall, AppStage, GlRenderStage> Application;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 1024, height = 768;
	WinRcViewerPtr viewer = std::make_shared<WinRcViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	opengl_sponza::Application app;
	app.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		app.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}


