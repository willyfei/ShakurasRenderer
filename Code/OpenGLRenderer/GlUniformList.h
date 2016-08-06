#pragma once
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;


class GlUniformList {
public:
	GlUniformList();

public:
	void setSharder(unsigned int sharder);

	void setUniform(const char* loc, int val);
	void setUniform(const char* loc, float val);
	void setUniform(const char* loc, const Vector2f& val);
	void setUniform(const char* loc, const Vector3f& val);
	void setUniform(const char* loc, const Vector4f& val);
	void setUniform(const char* loc, const Matrix44f& val);

private:
	unsigned int sharder_;
};


SHAKURAS_END;