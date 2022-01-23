/**
 * @file cMapLoader.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class loads a bit-map image and creates a map based on colours.
 */

#ifndef _cMapLoader_HG_
#define _cMapLoader_HG_

#include <gameMath.h>
#include <map>
#include <string>

class cMapLoader {
public:
	enum eTileType {
		TRAVERSABLE,
		NON_TRAVERSABLE,
		START,
		HOME_BASE,
		RESOURCE,
		UNKNOWN
	};

	unsigned char* mapData;
	std::map<eTileType, glm::vec3> tileMap;

	void initialize();
	unsigned int getMapWidth();
	unsigned int getMapHeight();

	unsigned char* loadMapData(const std::string& fileName);
	bool isValidTile(float r, float g, float b, eTileType tile);
	static bool flipMap(unsigned char* inBuffer, unsigned int width, unsigned int height);

	void printMap();

private:
	unsigned int mMapWidth;
	unsigned int mMapHeight;
	unsigned int mMapPixelCount;

	char mGetColourCharacter(unsigned char r, unsigned char g, unsigned char b);
};

#endif // !_cMapLoader_HG_

