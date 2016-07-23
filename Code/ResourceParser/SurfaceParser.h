#pragma once
#include "ResUtility.h"
#include "Core/MathAndGeometry.h"
#include "Core/Vertex.h"
#include "Core/Fragment.h"
#include "Core/Surface.h"
#include <vector>
#include <array>


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SurfacePtr GridSurface();
RESPARSER_DLL shakuras::SurfacePtr LoadSurface(std::string filepath, bool isrelpath = true);


#pragma warning(pop) 