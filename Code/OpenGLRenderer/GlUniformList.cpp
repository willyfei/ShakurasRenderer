#include "GlUniformList.h"


SHAKURAS_BEGIN;


GlUniformList::GlUniformList() {
	sharder_ = 0;
}


void GlUniformList::setSharder(unsigned int sharder) {
	sharder_ = sharder;
}


void GlUniformList::setUniform(const char* loc, int val) {

}


void GlUniformList::setUniform(const char* loc, float val) {

}


void GlUniformList::setUniform(const char* loc, const Vector2f& val) {

}


void GlUniformList::setUniform(const char* loc, const Vector3f& val) {

}


void GlUniformList::setUniform(const char* loc, const Vector4f& val) {

}


void GlUniformList::setUniform(const char* loc, const Matrix44f& val) {

}


SHAKURAS_END;