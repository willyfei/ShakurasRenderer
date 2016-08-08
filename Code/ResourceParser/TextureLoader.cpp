#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"


void* GridTexture(int& width, int& height) {
	uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t) * 256 * 256);

	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			buffer[j * 256 + i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}

	width = 256;
	height = 256;

	return buffer;
}


void* LoadTexture(std::string filepath, bool isrelpath, int& width, int& height) {
	if (isrelpath) {
		filepath = _FSPFX absolute(filepath, ResourceDir()).string();
	}

	int comp = 0;
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &comp, 4);
	if (!data) {
		return nullptr;
	}

	uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t) * width * height);

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			unsigned char* pp = data + 4 * (j * width + i);
			buffer[0] = pp[2];
			buffer[1] = pp[1];
			buffer[2] = pp[0];
			buffer[3] = pp[3];
		}
	}

	stbi_image_free(data);

	return buffer;
}