#pragma once
#include "ResUtility.h"
#include "Core/MathAndGeometry.h"
#include "Core/Soft/Vertex.h"
#include "Core/Soft/Fragment.h"
#include "Core/Soft/Surface.h"
#include <vector>
#include <array>


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SurfacePtr GridSurface();
RESPARSER_DLL shakuras::SurfacePtr LoadSurface(std::string filepath, bool isrelpath = true);


#pragma warning(pop) 