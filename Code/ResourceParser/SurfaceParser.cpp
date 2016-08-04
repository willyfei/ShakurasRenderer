#include "SurfaceParser.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"


using namespace shakuras;


SoftSurfacePtr GridSurface() {
	static uint32_t pixels[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			pixels[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}

	SoftSurfacePtr tex = std::make_shared<SoftSurface>();
	tex->reset(256, 256, &(pixels[0][0]));

	return tex;
}


SoftSurfacePtr LoadSurface(std::string filepath, bool isrelpath) {
	if (isrelpath) {
		filepath = _FSPFX absolute(filepath, ResourceDir()).string();
	}

	int x, y;
	int comp = 0;
	stbi_uc* data = stbi_load(filepath.c_str(), &x, &y, &comp, 4);
	if (!data) {
		return nullptr;
	}

	for (int j = 0; j < y; ++j) {
		for (int i = 0; i < x; ++i) {
			unsigned char* pp = data + 4 * (j*x + i);
			std::swap(pp[0], pp[2]);
		}
	}

	SoftSurfacePtr tex = std::make_shared<SoftSurface>();
	tex->reset(x, y, (uint32_t*)data);

	stbi_image_free(data);

	return tex;
}