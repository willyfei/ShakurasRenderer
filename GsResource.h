#pragma once
#include "MathAndGeometry.h"
#include "GsVertex.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//primitive资源
void GenerateCube(std::vector<GsVertex>& verts, std::vector<std::array<int, 3> >& itris);


//texture资源
GsTextureU32Ptr GridTexture();
GsTextureU32Ptr LoadTexture(std::string filepath);


SHAKURAS_END;