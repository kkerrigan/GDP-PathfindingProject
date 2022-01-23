/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cDllLoader.h"

cDllLoader::cDllLoader() {

	this->mDll = nullptr;
}

bool cDllLoader::loadDll(const std::string dllName) {

	// if there is already a DLL load, unload it
	if (this->mDll != nullptr) {
		this->unloadDll();
	}

	this->mDll = LoadLibrary(dllName.c_str());

	return this->mDll != nullptr;
}

void cDllLoader::unloadDll() {

	FreeLibrary(this->mDll);
	this->mDll = nullptr;

	return;
}