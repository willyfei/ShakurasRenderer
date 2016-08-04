#pragma once
#include "ResUtility.h"
#include "Renderer/MathAndGeometry.h"
#include "Renderer/Soft/SoftVertex.h"
#include "Renderer/Soft/SoftFragment.h"
#include "Renderer/Soft/SoftSurface.h"
#include <vector>
#include <array>


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SoftSurfacePtr GridSurface();
RESPARSER_DLL shakuras::SoftSurfacePtr LoadSurface(std::string filepath, bool isrelpath = true);


#pragma warning(pop) 