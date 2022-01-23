/**
 * @file PathingStructs.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This file contains various structs that are used for pathing.
 * This includes both path following and path findling.
 */

#ifndef _PathingStructs_HG_
#define _PathingStructs_HG_

#include <gameMath.h>
#include <vector>

struct sPathNode {
	sPathNode(glm::vec3 position) : position(position) {}
	glm::vec3 position;
};

struct sPath {
	std::vector<sPathNode> pathNodes;
};

enum eNeighbor {
	TOP_LEFT = 0,
	TOP_CENTRE = 1,
	TOP_RIGHT = 2,
	LEFT = 3,
	SELF = 4,
	RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_CENTRE = 7,
	BOTTOM_RIGHT = 8
};

struct sPathFindingNode {
	
	glm::vec3 position;
	bool bIsTraversable;
	bool bIsHomeBase;
	bool bIsGatherer;
	bool bIsResource;
	bool bInUse;
	float currentDistance;
	float currentDistanceFromNode;
	float currentNodeWeight;

	sPathFindingNode* parentNode;
	sPathFindingNode* neighbors[9];

	sPathFindingNode();
	
	sPathFindingNode* getNeighbor(eNeighbor index);
	void setNeighbor(eNeighbor index, sPathFindingNode* node);
};

struct sPathFinding {
	std::vector<sPathFindingNode*> pathNodes;
	std::vector<glm::vec3> pathPositions;
	float pathLength;
};


#endif // !_PathingStructs_HG_

