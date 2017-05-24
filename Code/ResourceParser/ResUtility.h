#pragma once
#include <string>
#include <Core/Config.hpp>

#ifdef _RESPARSER_DLL
#    define RESPARSER_DLL   KLAYGE_SYMBOL_EXPORT
#else
#    define RESPARSER_DLL   KLAYGE_SYMBOL_IMPORT
#endif

void LoadResourceDir(std::string cfg_fname = "ResourceDir.cfg");

_FSPFX path ResourceDir();

RESPARSER_DLL void ResFree(void* buffer);