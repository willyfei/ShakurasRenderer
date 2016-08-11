#pragma once
#include "GlRendererDll.h"
#include "Core/MathAndGeometry.h"
#include "GlMipmap.h"
#include <string>
#include <vector>
#include <map>

#pragma warning(push)
#pragma warning(disable:4251)


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlAbsProgram {
public:
	virtual ~GlAbsProgram() {}

public:
	virtual void use() = 0;
	virtual void prepare() = 0;
};


SHAKURAS_SHARED_PTR(GlAbsProgram);


class GLRENDERER_DLL GlProgram : public GlAbsProgram {
public:
	GlProgram();
	virtual ~GlProgram() {}

public:
	bool initSharder(const char* vs_src, const char* fs_src, const std::vector<std::string>& attrib_locs);

	void setUniform1f(const char* loc, float val);
	void setUniform2fv(const char* loc, const float* val);
	void setUniform3fv(const char* loc, const float* val);
	void setUniform4fv(const char* loc, const float* val);
	void setUniformMatrix3fv(const char* loc, const float* val);
	void setUniformMatrix4fv(const char* loc, const float* val);
	void setUniformTexture2D(const char* loc, GlMipmapPtr mipmap);

	virtual void use();
	virtual void prepare() {}

protected:
	unsigned int prog_handle_;
};


SHAKURAS_SHARED_PTR(GlProgram);


SHAKURAS_END;

#pragma warning(pop)