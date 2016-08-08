#include "GlProgram.h"
#include "gl\glew.h"


SHAKURAS_BEGIN;


GlProgram::GlProgram() {
	program_ = 0;
}


void LoadShaderSrc(const char *sharder_src, GLuint shader)
{
	GLchar* fs_string_ptr[1];

	fs_string_ptr[0] = (GLchar*)sharder_src;
	glShaderSource(shader, 1, (const GLchar **)fs_string_ptr, NULL);
}


bool GlProgram::create(const char* vs_src, const char* fs_src, const std::vector<std::string>& attrib_locs) {
	program_ = 0;

	// Create shader objects
	GLuint vs_handle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs_handle = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. 
	LoadShaderSrc(vs_src, vs_handle);
	LoadShaderSrc(fs_src, fs_handle);

	// Compile them
	glCompileShader(vs_handle);
	glCompileShader(fs_handle);

	// Check for errors
	GLint test_rv = 0;
	glGetShaderiv(vs_handle, GL_COMPILE_STATUS, &test_rv);
	if (test_rv == GL_FALSE) {
		glDeleteShader(vs_handle);
		glDeleteShader(fs_handle);
		return false;
	}

	glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &test_rv);
	if (test_rv == GL_FALSE) {
		glDeleteShader(vs_handle);
		glDeleteShader(fs_handle);
		return false;
	}

	// Link them - assuming it works...
	GLuint prog_handle = glCreateProgram();
	glAttachShader(prog_handle, vs_handle);
	glAttachShader(prog_handle, fs_handle);

	// List of attributes
	for (size_t i = 0; i != attrib_locs.size(); i++) {
		glBindAttribLocation(prog_handle, (int)i, attrib_locs[i].c_str());
	}

	glLinkProgram(prog_handle);

	// These are no longer needed
	glDeleteShader(vs_handle);
	glDeleteShader(fs_handle);
	
	// Make sure link worked too
	glGetProgramiv(prog_handle, GL_LINK_STATUS, &test_rv);
	if (test_rv == GL_FALSE) {
		glDeleteProgram(prog_handle);
		return false;
	}

	program_ = prog_handle;

	return true;
}


void GlProgram::setUniform(std::string loc, float val) {
	GLint uid = glGetUniformLocation(program_, loc.c_str());
	glUniform1f(uid, val);
}


void GlProgram::setUniform(std::string loc, const Vector2f& val) {
	GLint uid = glGetUniformLocation(program_, loc.c_str());
	glUniform2f(uid, val.x, val.y);
}


void GlProgram::setUniform(std::string loc, const Vector3f& val) {
	GLint uid = glGetUniformLocation(program_, loc.c_str());
	glUniform3f(uid, val.x, val.y, val.z);
}


void GlProgram::setUniform(std::string loc, const Vector4f& val) {
	GLint uid = glGetUniformLocation(program_, loc.c_str());
	glUniform4f(uid, val.x, val.y, val.z, val.w);
}


void GlProgram::setUniform(std::string loc, const Matrix44f& val) {
	GLint uid = glGetUniformLocation(program_, loc.c_str());

	std::vector<float> mat;
	mat.reserve(16);
	std::copy(val.m[0].begin(), val.m[0].end(), std::back_inserter(mat));
	std::copy(val.m[1].begin(), val.m[1].end(), std::back_inserter(mat));
	std::copy(val.m[2].begin(), val.m[2].end(), std::back_inserter(mat));
	std::copy(val.m[3].begin(), val.m[3].end(), std::back_inserter(mat));
	glUniformMatrix4fv(uid, 1, GL_FALSE, mat.data());
}


SHAKURAS_END;