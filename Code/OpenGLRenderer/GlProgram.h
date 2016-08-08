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
	bool create(const char* vsharder, const char* fsharder, const std::vector<std::string>& attrib_locs);

	void setUniform(std::string loc, float val);
	void setUniform(std::string loc, const Vector2f& val);
	void setUniform(std::string loc, const Vector3f& val);
	void setUniform(std::string loc, const Vector4f& val);
	void setUniform(std::string loc, const Matrix44f& val);

private:
	unsigned int program_;
};


SHAKURAS_END;