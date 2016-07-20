#pragma once
#include "MathAndGeometry.h"
#include "Vertex.h"
#include "Fragment.h"
#include "Surface.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//texture×ÊÔ´
SurfacePtr GridSurface();
SurfacePtr LoadSurface(std::string filepath);


SHAKURAS_END;