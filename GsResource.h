#pragma once
#include "UtilityAndMath.h"
#include "GsPrimitive.h"
#include "GsTexture.h"
#include <vector>


SHAKURAS_BEGIN;


//primitive资源
void GenerateCube(std::vector<GsTriangle>& prims);


//texture资源
GsTextureU32Ptr GenerateGrid();


SHAKURAS_END;