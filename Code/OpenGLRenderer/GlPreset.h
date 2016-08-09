#pragma once
#include "GlProgram.h"
#include <string>

#pragma warning(push)
#pragma warning(disable:4251)


SHAKURAS_BEGIN;


//典型 Fixed-Function Pipeline 的 Phong 着色
class GLRENDERER_DLL GlPhongProgram : public GlProgram {
public:
	bool initialize();

public:
	enum AttribIndex {
		kVertPos = 0, kVertNormal = 1, kVertUV = 2, kAttribCount = 3
	};

	//uniforms
	void setAmbient(const Vector3f& ambient);
	void setDiffuse(const Vector3f& diffuse);
	void setSpecular(const Vector3f& specular);
	void setModelTrsf(const Matrix44f& model_trsf);
	void setMvpTrsf(const Matrix44f& mvp_trsf);
	void setLightPos(const Vector3f& light_pos);
	void setEyePos(const Vector3f& eye_pos);
	void setTexture(GlMipmapPtr tex);

private:
	const char* vertexSharderSource();
	const char* fragmentSharderSource();
};


SHAKURAS_SHARED_PTR(GlPhongProgram);


SHAKURAS_END;

#pragma warning(pop)