#pragma once
#include "ResUtility.h"
#include "Core/MathAndGeometry.h"
#include "SoftRenderer/SoftVertex.h"
#include "SoftRenderer/SoftFragment.h"
#include "SoftRenderer/SoftSurface.h"
#include <vector>
#include <array>


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SoftSurfacePtr GridSurface();
RESPARSER_DLL shakuras::SoftSurfacePtr LoadSurface(std::string filepath, bool isrelpath = true);


#pragma warning(pop) 