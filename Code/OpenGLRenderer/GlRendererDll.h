#pragma once

#if defined(__ANDROID__)
#ifdef _GLRENDERER_DLL
#define GLRENDERER_DLL   __attribute__((__visibility__("default")))
#else
#define GLRENDERER_DLL
#endif
#else
#ifdef _GLRENDERER_DLL
#define GLRENDERER_DLL   __declspec(dllexport)
#else
#define GLRENDERER_DLL   __declspec(dllimport)
#endif
#endif