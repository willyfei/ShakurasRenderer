#pragma once
#include "MathAndGeometry.h"
#include "Vertex.h"
#include "Fragment.h"
#include "Texture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//texture×ÊÔ´
SurfacePtr GridTexture();
SurfacePtr LoadTexture(std::string filepath);


SHAKURAS_END;