#include "GlMipmap.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"


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


SHAKURAS_END;