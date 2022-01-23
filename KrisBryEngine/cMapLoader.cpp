/**
 * @file cMapLoader.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cMapLoader class.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "cMapLoader.h"

 /**
 * initialize
 * @brief This function sets up the map loader by defining the tile types.
 * @return void.
 */
void cMapLoader::initialize() {

	this->tileMap[eTileType::TRAVERSABLE] = glm::vec3(255.0f, 255.0f, 255.0f);
	this->tileMap[eTileType::NON_TRAVERSABLE] = glm::vec3(0.0f, 0.0f, 0.0f);
	this->tileMap[eTileType::START] = glm::vec3(0.0f, 255.0f, 0.0f);
	this->tileMap[eTileType::HOME_BASE] = glm::vec3(0.0f, 0.0f, 255.0f);
	this->tileMap[eTileType::RESOURCE] = glm::vec3(255.0f, 0.0f, 0.0f);

	return;
}

/**
* getMapWidth
* @brief A getter for the width of the loaded map.
* @return An unsigned int representing the width of the map.
*/
unsigned int cMapLoader::getMapWidth() {

	return this->mMapWidth;
}

/**
* getMapHeight
* @brief A getter for the height of the loaded map.
* @return An unsigned int representing the height of the map.
*/
unsigned int cMapLoader::getMapHeight() {

	return this->mMapHeight;
}

/**
* loadMapData
* @brief This function opens the BMP file and reads in its data.
* @param const string& fileName - the name of the BMP file.
* @return An unsigned char* representing the map data.
*/
unsigned char* cMapLoader::loadMapData(const std::string& fileName) {

	std::string fullPath = "./assets/bitmaps/" + fileName;

	FILE* file = fopen(fullPath.c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, file); //header (54 bytes)

	//get the width and height from the header
	this->mMapWidth = *reinterpret_cast<unsigned int*>(&info[18]);
	this->mMapHeight = *reinterpret_cast<unsigned int*>(&info[22]);
	this->mMapPixelCount = this->mMapWidth * this->mMapHeight;

	//calculate the size (number of pixels * 3 color weights(rgb)
	const int size = 3 * this->mMapWidth * this->mMapHeight;

	if (this->mapData != nullptr) {

		delete this->mapData;
	}

	this->mapData = new unsigned char[size]; //3 bytes per pixel
	fread(this->mapData, sizeof(unsigned char), size, file); //read all the data at once
	fclose(file);

	for (size_t index = 0; index < size; index += 3) {

		const unsigned char temp = this->mapData[index];
		this->mapData[index] = this->mapData[index + 2];
		this->mapData[index + 2] = temp;
	}

	cMapLoader::flipMap(this->mapData, this->mMapWidth, this->mMapHeight);

	return this->mapData;
}

/**
* isValidTile
* @brief This function determines if the data read at a given index is a valid tile.
* @param float r - the r value of the bitmap.
* @param float g - the g value of the bitmap.
* @param float b - the b value of the bitmap.
* @param eTileType tile - The tile type being testing against the bitmap values.
* @return a bool representing whether the values create a valid tile type.
*/
bool cMapLoader::isValidTile(float r, float g, float b, eTileType tile) {

	// See if the passed in type is in the map
	const std::map<eTileType, glm::vec3>::iterator it = this->tileMap.find(tile);
	if (it == this->tileMap.end()) return false;

	return r == it->second.r && g == it->second.g && b == it->second.b;
}

/**
* flipMap
* @brief This function orients the BMP file to the way it was created.
* @param unsigned char* inBuffer - the data of the map.
* @param unsigned int width - the width of the map.
* @param unsigned int height - the height of the map.
* @return A bool representing whether the re-orienting of the map was successful.
*/
bool cMapLoader::flipMap(unsigned char* inBuffer, unsigned int width, unsigned int height) {

	// Check that the buffer actually has data
	if (inBuffer == nullptr) return false;

	const unsigned buffer_size = width * 3;
	unsigned char* tb1 = new unsigned char[buffer_size];
	if (tb1 == nullptr) return false;

	unsigned char* tb2 = new unsigned char[buffer_size];
	if (tb2 == nullptr) {
		
		delete[] tb1;
		return false;
	}

	long off1 = 0;
	long off2 = 0;

	for (int row_count = 0; row_count < (height) / 2; row_count++) {
		
		off1 = row_count * buffer_size;
		off2 = ((height - 1) - row_count) * buffer_size;

		memcpy(tb1, inBuffer + off1, buffer_size * sizeof(unsigned char));
		memcpy(tb2, inBuffer + off2, buffer_size * sizeof(unsigned char));
		memcpy(inBuffer + off1, tb2, buffer_size * sizeof(unsigned char));
		memcpy(inBuffer + off2, tb1, buffer_size * sizeof(unsigned char));
	}

	delete[] tb1;
	delete[] tb2;

	return true;

}

/**
* printMap
* @brief Prints the loaded BMP image to the console window.
* @return void.
*/
void cMapLoader::printMap() {

	int index = 0;
	for (unsigned long x = 0; x < this->mMapWidth; x++) {

		for (unsigned long y = 0; y < this->mMapHeight; y++) {

			printf("%c", this->mGetColourCharacter(this->mapData[index++], this->mapData[index++], this->mapData[index++]));
		}
		printf("\n");
	}

	return;
}

/**
* mGetColourCharacter
* @brief Retrieves the corresponding letter for the given values
* @param unsigned char r - the R value of the BMP.
* @param unsigned char g - the G value of the BMP.
* @param unsigned char b - the B value of the BMP.
* @return A char representing the colour character.
*/
char cMapLoader::mGetColourCharacter(unsigned char r, unsigned char g, unsigned char b) {

	if (r == 255.0f && g == 0.0f && b == 0.0f) return 'r';
	if (r == 0.0f && g == 255.0f && b == 0.0f) return 'g';
	if (r == 0.0f && g == 0.0f && b == 255.0f) return 'b';
	if (r == 255.0f && g == 255.0f && b == 255.0f) return 'w';
	if (r == 0.0f && g == 0.0f && b == 0.0f) return '_';
	return 'x';
}
