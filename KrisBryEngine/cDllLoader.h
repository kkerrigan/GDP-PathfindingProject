/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cDllLoader_HG_
#define _cDllLoader_HG_

#include <Windows.h>
#include <string>

class cDllLoader {
public:
	cDllLoader();
	bool loadDll(const std::string dllName);
	void unloadDll();

	template<typename T>
	T getProcAddress(const std::string functionName) {
		return reinterpret_cast<T>(GetProcAddress(this->mDll, functionName.c_str()));
	}

private:
	HINSTANCE mDll;
};

#endif
