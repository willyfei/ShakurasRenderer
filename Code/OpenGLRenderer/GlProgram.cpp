#include "GlProgram.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"
#include "gl\wglew.h"


SHAKURAS_BEGIN;


GlProgram::GlProgram() {
	prog_handle_ = 0;
}


void LoadShaderSrc(const char *sharder_src, GLuint shader) {
	GLchar* fs_string_ptr[1];

	fs_string_ptr[0] = (GLchar*)sharder_src;
	glShaderSource(shader, 1, (const GLchar **)fs_string_ptr, NULL);
}


bool GlProgram::initSharder(const char* vs_src, const char* fs_src, const std::vector<std::string>& attrib_locs) {
	prog_handle_ = 0;
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return false;
	}

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
		char sharder_info[4096];
		GLint len = 0;
		glGetShaderInfoLog(vs_handle, 4096, &len, sharder_info);
		printf_s(sharder_info);
		getchar();

		glDeleteShader(vs_handle);
		glDeleteShader(fs_handle);
		return false;
	}

	glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &test_rv);
	if (test_rv == GL_FALSE) {
		char sharder_info[4096];
		GLint len = 0;
		glGetShaderInfoLog(fs_handle, 4096, &len, sharder_info);
		printf_s(sharder_info);
		getchar();

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

	prog_handle_ = prog_handle;

	return true;
}


void GlProgram::use() {
	glUseProgram(prog_handle_);
}


void GlProgram::setUniform1f(const char* loc, float val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniform1f(uid, val);
}


void GlProgram::setUniform2fv(const char* loc, const float* val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniform2fv(uid, 1, val);
}


void GlProgram::setUniform3fv(const char* loc, const float* val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniform3fv(uid, 1, val);
}


void GlProgram::setUniform4fv(const char* loc, const float* val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniform4fv(uid, 1, val);
}


void GlProgram::setUniformMatrix3fv(const char* loc, const float* val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniformMatrix3fv(uid, 1, GL_FALSE, val);
}


void GlProgram::setUniformMatrix4fv(const char* loc, const float* val) {
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniformMatrix4fv(uid, 1, GL_FALSE, val);
}


void GlProgram::setUniformTexture2D(const char* loc, GlMipmapPtr mipmap) {
	glBindTexture(GL_TEXTURE_2D, mipmap->tex_id_);
	GLint uid = glGetUniformLocation(prog_handle_, loc);
	glUniform1i(uid, 0);
}


SHAKURAS_END;