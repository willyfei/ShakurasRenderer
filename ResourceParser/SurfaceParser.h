#pragma once
#include "Core/MathAndGeometry.h"
#include "Core/Vertex.h"
#include "Core/Fragment.h"
#include "Core/Surface.h"
#include <vector>
#include <array>


#ifdef _RESPARSER_DLL
#    define RESPARSER_DLL   __declspec(dllexport)
#else
#    define RESPARSER_DLL   __declspec(dllimport)
#endif


#pragma warning(push) 
#pragma warning(disable:4251)


//texture×ÊÔ´
RESPARSER_DLL shakuras::SurfacePtr GridSurface();
RESPARSER_DLL shakuras::SurfacePtr LoadSurface(std::string filepath);


#pragma warning(pop) 