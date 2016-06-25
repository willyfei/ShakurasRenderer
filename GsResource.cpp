#include "stdafx.h"
#include "GsResource.h"


SHAKURAS_BEGIN;


void GenerateCube(std::vector<GsTriangle>& prims) {
	static GsVertex mesh[8]{
		{ { 1, -1,  1, 1 },{ 0, 0 },{ 1.0f, 0.2f, 0.2f }, 1 },
		{ { -1, -1,  1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 0.2f }, 1 },
		{ { -1,  1,  1, 1 },{ 1, 1 },{ 0.2f, 0.2f, 1.0f }, 1 },
		{ { 1,  1,  1, 1 },{ 1, 0 },{ 1.0f, 0.2f, 1.0f }, 1 },
		{ { 1, -1, -1, 1 },{ 0, 0 },{ 1.0f, 1.0f, 0.2f }, 1 },
		{ { -1, -1, -1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 1.0f }, 1 },
		{ { -1,  1, -1, 1 },{ 1, 1 },{ 1.0f, 0.3f, 0.3f }, 1 },
		{ { 1,  1, -1, 1 },{ 1, 0 },{ 0.2f, 1.0f, 0.3f }, 1 },
	};

	auto draw_plane = [&](int a, int b, int c, int d) {
		GsVertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
		p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
		p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
		prims.push_back({ p1, p2, p3 });
		prims.push_back({ p3, p4, p1 });
	};

	draw_plane(0, 1, 2, 3);
	draw_plane(4, 5, 6, 7);
	draw_plane(0, 4, 5, 1);
	draw_plane(1, 5, 6, 2);
	draw_plane(2, 6, 7, 3);
	draw_plane(3, 7, 4, 0);
}



GsTextureU32Ptr GenerateGrid()
{
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


SHAKURAS_END;