#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <optional>
#include <unordered_map>
#include <strsafe.h>

#include <extensions_functions.h>
#include "stuctures.h"


const char* functions_name[] = {
	"init","update","destroy"
};

enum functions_enum {
	init_f, update_f, destroy_f, maxnum
};

std::optional<std::unordered_map<std::string, library>> LoadLibs() {
	auto defultErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	using namespace std::filesystem;
	std::unordered_map<std::string, library> libs;
	libs.reserve(10);

	if (!std::filesystem::is_directory("extensions"))
	{
		std::cout << "Couldn't find \"extensions\" directory.\n";
		return {};
	}

	for (auto& a : directory_iterator("extensions")) {
		if (a.is_regular_file()) {
			auto fullname = a.path().filename().string();
			if (a.path().extension().string().compare(".dll") != 0)
				continue;
			auto name = a.path().stem().string();
			auto& currLib = libs[name];


			//If the function fails, the return value is NULL.To get extended error information, call GetLastError.
			HMODULE  library = LoadLibraryExA(a.path().string().c_str(), NULL, NULL);

			if (!library) {
				std::cout << "Couldn't load [" << fullname << "]: With error code (" << GetLastError() << ")\n";
				continue;
			}
			currLib.library = library;
			for (unsigned int i = 0; i < functions_enum::maxnum; i++) {
				FARPROC func = GetProcAddress(library, functions_name[i]);
				if (func) {
					switch (i)
					{
						case functions_enum::init_f: {
							currLib.init = (_init*)func;
							break;
						}
						case functions_enum::destroy_f: {
							currLib.destroy = (_destroy*)func;
							break;
						}
						case functions_enum::update_f: {
							currLib.update = (_update*)func;
							break;
						}
						default:
							// Log here
							// @NOTE: this is important becuase of the stupidity of c++ atm
							//		  we don't have reflection
							break;
					}
				}
			}
		}
	}
	SetErrorMode(defultErrorMode);
	return libs;
}


inline
void FreeLibs(std::unordered_map<std::string, library> libs) {
	for (auto& [name, lib] : libs) {
		// @TODO:  check what return from FreeLibrary
		if (lib.destroy) lib.destroy();
		FreeLibrary(lib.library);
	}
}

#define TEST
#ifdef TEST

int main(int argc, char** argv) {

	//Load All libs
	auto olibs = LoadLibs();
	// This is for test
	if (olibs.has_value()) {
		auto libs = olibs.value();
		for (auto& [name, lib] : libs) {
			if (lib.init)	lib.init();
			if (lib.update) {
				std::cout << lib.update(2, 2) << "\n";
			}
		}
		// unload all libraries
		FreeLibs(libs);
	}
	system("pause");
	// 0 denote successful exit
	return 0;
}

#endif // TEST