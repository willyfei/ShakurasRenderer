#pragma once
#include <string>
#include <filesystem>


#ifdef _RESPARSER_DLL
#    define RESPARSER_DLL   __declspec(dllexport)
#else
#    define RESPARSER_DLL   __declspec(dllimport)
#endif


void LoadResourceDir(std::string cfg_fname = "ResourceDir.cfg");


_FSPFX path ResourceDir();


RESPARSER_DLL void ResFree(void* buffer);