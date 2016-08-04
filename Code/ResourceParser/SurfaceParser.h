#pragma once
#include "ResUtility.h"
#include "Renderer/MathAndGeometry.h"
#include "Renderer/Soft/Vertex.h"
#include "Renderer/Soft/Fragment.h"
#include "Renderer/Soft/Surface.h"
#include <vector>
#include <array>


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SurfacePtr GridSurface();
RESPARSER_DLL shakuras::SurfacePtr LoadSurface(std::string filepath, bool isrelpath = true);


#pragma warning(pop) 