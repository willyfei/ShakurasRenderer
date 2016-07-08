#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//texture×ÊÔ´
GsTextureU32Ptr GridTexture();
GsTextureU32Ptr LoadTexture(std::string filepath);


SHAKURAS_END;