#include "GlPreset.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"
#include "gl\wglew.h"


SHAKURAS_BEGIN;


bool GlColorProgram::initialize() {
	std::vector<std::string> attrib_locs;
	attrib_locs.push_back("vertPos");
	return initSharder(vertexSharderSource(), fragmentSharderSource(), attrib_locs);
}


void GlColorProgram::setColor(const Vector3f& color) {
	setUniform4fv("color", color.xyzw(1.0f).data());
}


const char* GlColorProgram::vertexSharderSource() {
	static const char * vs_src =
		"attribute vec4 vertPos;"
		"void main(void) "
		"{"
		"	gl_Position = vertPos;"
		"}";

	return vs_src;
}


const char* GlColorProgram::fragmentSharderSource() {
	static const char * fs_src =
		"uniform vec4 color;"
		"void main(void) "
		"{"
		"	gl_FragColor = color;"
		"}";

	return fs_src;
}



bool GlPhongProgram::initialize() {
	std::vector<std::string> attrib_locs;
	attrib_locs.push_back("vertPos");
	attrib_locs.push_back("vertNormal");
	attrib_locs.push_back("vertUV");
	return initSharder(vertexSharderSource(), fragmentSharderSource(), attrib_locs);
}


void GlPhongProgram::setAmbient(const Vector3f& ambient) {
	setUniform3fv("ambient", ambient.data());
}


void GlPhongProgram::setDiffuse(const Vector3f& diffuse) {
	setUniform3fv("diffuse", diffuse.data());
}


void GlPhongProgram::setSpecular(const Vector3f& specular) {
	setUniform3fv("specular", specular.data());
}


void GlPhongProgram::setModelTrsf(const Matrix44f& model_trsf) {
	setUniformMatrix3fv("mTrsf3", model_trsf.mat3().flat().data());
}


void GlPhongProgram::setMvpTrsf(const Matrix44f& mvp_trsf) {
	setUniformMatrix4fv("mvpTrsf", mvp_trsf.flat().data());
}


void GlPhongProgram::setLightPos(const Vector3f& light_pos) {
	setUniform3fv("lightPos", light_pos.data());
}


void GlPhongProgram::setEyePos(const Vector3f& eye_pos) {
	setUniform3fv("eyePos", eye_pos.data());
}


void GlPhongProgram::setTexture(GlMipmapPtr tex) {
	setUniformTexture2D("colorMap", tex);
}


const char* GlPhongProgram::vertexSharderSource() {
	static const char * vs_src =
		"uniform mat4 mvpTrsf;"
		"uniform mat3 mTrsf3;"
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
		"	varyingNormal = mTrsf3 * vertNormal;"
		"	varyingUV = vertUV;"
		"	varyingLightDir = lightPos - vertPos.xyz;"
		"	varyingEyeDir = eyePos - vertPos.xyz;"
		"	gl_Position = mvpTrsf * vertPos;"
		"}";

	return vs_src;
}


const char* GlPhongProgram::fragmentSharderSource() {
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
		"	float illumDifuse = clamp(dot(lightDir, norm), 0.0, 1.0);"
		"	float illumSpecular = clamp(dot(reflect(lightDir, norm), eyeDir), 0.0, 1.0);"
		"	vec3 illum = ambient + diffuse * illumDifuse + specular * illumSpecular;"
		""
		"	vec4 texColor = texture(colorMap, varyingUV.st);"
		"	float r = clamp(texColor.x * illum.x, 0.0, 1.0);"
		"	float g = clamp(texColor.y * illum.y, 0.0, 1.0);"
		"	float b = clamp(texColor.z * illum.z, 0.0, 1.0);"
		""
		"	gl_FragColor = vec4(r, g, b, 1.0);"
		"}";

	return fs_src;
}


SHAKURAS_END;