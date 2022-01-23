/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cDebugUtilities_HG_
#define _cDebugUtilities_HG_

#include "cEntity.h"

class cDebugUtilities {
public:
	static void renderCameraInfo();
	static void renderFlockInfo();
	static void clearBuffer(char* buffer, size_t size);
	static void displaySceneAABBs(cEntity* pCube);
};


#endif // !_cDebugUtilities_HG_

