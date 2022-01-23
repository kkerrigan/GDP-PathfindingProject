#include <cBasicTextureManager.h>
#include <iostream>

void loadTextures() {
	cBasicTextureManager* pBasicTextureManager = cBasicTextureManager::getInstance();
	pBasicTextureManager->SetBasePath("./assets/textures/");

	
	if (!pBasicTextureManager->Create2DTextureFromBMPFile("gravel.bmp", true)) {
		std::cout << "Did not load gravel.bmp" << std::endl;
	}

	//if (!pBasicTextureManager->Create2DTextureFromBMPFile("debug_grid.bmp", true)) {
	//	std::cout << "Did not load debug_grid.bmp" << std::endl;
	//}

	//if (!pBasicTextureManager->Create2DTextureFromBMPFile("wall.bmp", true)) {
	//	std::cout << "Did not load wall.bmp" << std::endl;
	//}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("sprial.bmp", true)) {
		std::cout << "Did not load sprial.bmp" << std::endl;
	}

	// Load cube maps
	pBasicTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;

	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("TropicalDayCubeMap",
		"TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",		// Alternate these
		"TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp", 			// Rotate these 90 degrees
		"TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString))
	{
		std::cout << "Loaded the Sunny Day cube map OK" << std::endl;
	}
	else {
		std::cout << "Error: sunny day cube map DIDN't load. On no!" << std::endl;
	}

	return;
}