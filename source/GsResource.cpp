#include "GsResource.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"

SHAKURAS_BEGIN;


GsTextureU32Ptr GridTexture() {
	static uint32_t pixels[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			pixels[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}

	GsTextureU32Ptr tex = std::make_shared<GsTextureU32>();
	tex->reset(&(pixels[0][0]), 256, 256);

	return tex;
}


GsTextureU32Ptr LoadTexture(std::string filepath) {
	int x, y;
	stbi_uc* data = stbi_load(filepath.c_str(), &x, &y, 0, 4);
	if (!data) {
		return nullptr;
	}

	for (int j = 0; j < y; ++j) {
		for (int i = 0; i < x; ++i) {
			unsigned char* pp = data + 4 * (j*x + i);
			std::swap(pp[0], pp[2]);
		}
	}

	GsTextureU32Ptr tex = std::make_shared<GsTextureU32>();
	tex->reset((uint32_t*)data, x, y);

	STBI_FREE(data);

	return tex;
}


SHAKURAS_END;