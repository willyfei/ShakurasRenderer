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


class GLRENDERER_DLL GlProgram {
public:
	GlProgram();
	virtual ~GlProgram() {}

public:
	bool initSharder(const char* vs_src, const char* fs_src, const std::vector<std::string>& attrib_locs);

	void use();

	void setUniform1f(const char* loc, float val);
	void setUniform2fv(const char* loc, const float* val);
	void setUniform3fv(const char* loc, const float* val);
	void setUniform4fv(const char* loc, const float* val);
	void setUniformMatrix3fv(const char* loc, const float* val);
	void setUniformMatrix4fv(const char* loc, const float* val);
	void setUniformTexture2D(const char* loc, GlMipmapPtr mipmap);

protected:
	unsigned int prog_handle_;
};


SHAKURAS_SHARED_PTR(GlProgram);


SHAKURAS_END;

#pragma warning(pop)