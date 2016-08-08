#pragma once
#include "Core/MathAndGeometry.h"
#include <string>
#include <vector>
#include <map>


SHAKURAS_BEGIN;


class GlProgram {
public:
	GlProgram();

public:
	bool create(const char* vs_src, const char* fs_src, const std::vector<std::string>& attrib_locs);

	void use();

	void setUniform1f(std::string loc, float val);
	void setUniform2fv(std::string loc, const float* val);
	void setUniform3fv(std::string loc, const float* val);
	void setUniform4fv(std::string loc, const float* val);
	void setUniformMatrix4fv(std::string loc, const float* val);
	void setUniformTexture2D(std::string loc, std::string tex_full_path);

	//÷ÿ‘ÿ£¨∂®÷∆uniform list
	virtual void prepare() = 0;

private:
	unsigned int prog_handle_;
};


SHAKURAS_SHARED_PTR(GlProgram);


SHAKURAS_END;