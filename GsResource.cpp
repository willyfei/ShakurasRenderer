#include "stdafx.h"
#include "GsResource.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"

SHAKURAS_BEGIN;


void GenerateCube(std::vector<GsVertexA8V4>& verts, std::vector<std::array<int, 3> >& itris) {
	static GsVertexA8V4 mesh[8]{
		{ { 1, -1,  1, 1 } },
		{ { -1, -1,  1, 1 } },
		{ { -1,  1,  1, 1 } },
		{ { 1,  1,  1, 1 } },
		{ { 1, -1, -1, 1 } },
		{ { -1, -1, -1, 1 } },
		{ { -1,  1, -1, 1 } },
		{ { 1,  1, -1, 1 } },
	};

	auto draw_plane = [&](int a, int b, int c, int d) {
		GsVertexA8V4 p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
		p1.varying[0] = 0, p1.varying[1] = 0, p2.varying[0] = 0, p2.varying[1] = 1;
		p3.varying[0] = 1, p3.varying[1] = 1, p4.varying[0] = 1, p4.varying[1] = 0;
		int index = verts.size();
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
		itris.push_back({ index, index + 1, index + 2 });

		index = verts.size();
		verts.push_back(p3);
		verts.push_back(p4);
		verts.push_back(p1);
		itris.push_back({ index, index + 1, index + 2 });
	};

	draw_plane(0, 1, 2, 3);
	draw_plane(4, 5, 6, 7);
	draw_plane(0, 4, 5, 1);
	draw_plane(1, 5, 6, 2);
	draw_plane(2, 6, 7, 3);
	draw_plane(3, 7, 4, 0);
}


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