#include "GlMipmap.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif


SHAKURAS_BEGIN;


unsigned int CreateGlMipmap(void* bits, int width, int height, int comp, int fmt) {
	if (!bits) {
		return 0;
	}
	
	GLuint tex_id = 0;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, fmt, GL_UNSIGNED_BYTE, bits);

	glGenerateMipmap(GL_TEXTURE_2D);

	return tex_id;
}


GlMipmap::GlMipmap() {
	tex_id_ = 0;
}


GlMipmap::~GlMipmap() {
	if (tex_id_ != 0) {
		glDeleteTextures(1, &tex_id_);
	}
}


void GlMipmap::load(void* bits, int width, int height) {
	tex_id_ = CreateGlMipmap(bits, width, height, 4, GL_RGBA);
}


GlMipmapPtr CreateGlMipmap(void* bits, int width, int height) {
	GlMipmapPtr mipmap = std::make_shared<GlMipmap>();
	mipmap->load(bits, width, height);
	return mipmap;
}


SHAKURAS_END;