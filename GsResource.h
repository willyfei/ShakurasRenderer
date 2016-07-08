#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//primitive资源
template<class Vert>
void GenerateCube(std::vector<Vert>& verts, std::vector<std::array<int, 3> >& itris) {
	static Vert mesh[8]{
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
		Vert p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

		p1.attrib.set(0, 0);
		p2.attrib.set(0, 1);
		p3.attrib.set(1, 1);
		p4.attrib.set(1, 0);

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


//texture资源
GsTextureU32Ptr GridTexture();
GsTextureU32Ptr LoadTexture(std::string filepath);


SHAKURAS_END;