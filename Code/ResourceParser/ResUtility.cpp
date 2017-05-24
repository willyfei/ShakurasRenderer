#include "ResUtility.h"
#include <fstream>
//#include <filesystem>


_FSPFX path g_resourcedir;


std::string& TrimSlash(std::string& str) {
	while (!str.empty() && (str.back() == '/' || str.back() == '\\')) {
		str.pop_back();
	}

	while (!str.empty() && (str.front() == '/' || str.front() == '\\')) {
		str = str.substr(1);
	}

	return str;
}


void LoadResourceDir(std::string cfg_fname) {
	std::ifstream fs(cfg_fname);
	if (fs) {
		std::string str;
		std::getline(fs, str);
		TrimSlash(str);

		_FSPFX path reldir = str;
		g_resourcedir = _FSPFX absolute(reldir);
	}
}


_FSPFX path ResourceDir() {
	return g_resourcedir;
}


void ResFree(void* buffer) {
	free(buffer);
}