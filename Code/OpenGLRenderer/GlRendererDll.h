#pragma once


#ifdef _GLRENDERER_DLL
#    define GLRENDERER_DLL   __declspec(dllexport)
#else
#    define GLRENDERER_DLL   __declspec(dllimport)
#endif