/**
 * @file cPathFinding.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class will implement the Dijkstra algorithn in order to find the 
 * shortest path for an entity.
 * As well as implement the A* algorithm to find the shortest path for an entity.
 */

#ifndef _cPathFinding_HG_
#define _cPathFinding_HG_

#include <gameMath.h>
#include <vector>

#include "PathingStructs.h"
#include "cMapLoader.h"

class cPathFinding {
public:
	sPathFinding path;
	
	sPathFinding findShortestDijkstraPath(glm::vec3 currentPosition, sPathFindingNode* startNode, sPathFindingNode* goalNode);
	sPathFinding findShortestAStarPath(glm::vec3 currentPosition, sPathFindingNode* startNode, sPathFindingNode* goalNode);

	sPathFindingNode* findClosestTileType(glm::vec3 startPosition, cMapLoader::eTileType tileType);

	sPathFindingNode* findClosestNode(glm::vec3 position);
	sPathFinding findGathererNodes();
	sPathFinding findResourceNodes();
	
	void findNodeNeighbors(unsigned int mapWidth, unsigned int mapHeight);

private:
	void mAddToOpenSet(std::vector<sPathFindingNode*>& openSet, sPathFindingNode* node);
	bool mRemoveFromOpenSet(std::vector<sPathFindingNode*>& openSet, sPathFindingNode* node);
	void mAddToClosedSet(std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* node);
	bool mRemoveFromClosedSet(std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* node);

	sPathFinding mCalculateDijkstraPath(std::vector<sPathFindingNode*>& openSet, std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* goalNode);
	sPathFinding mCalculateAStarPath(std::vector<sPathFindingNode*>& openSet, std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* goalNode);
	float mCalculateEuclideanHeuristic(sPathFindingNode* currentNode, sPathFindingNode* goalNode);

	bool mIsNodeInSet(std::vector<sPathFindingNode*>& set, sPathFindingNode* node);
	bool mIsValidDiagonal(sPathFindingNode* closestNode, int diagonalIndex);
};

#endif // !_cDijkstraPathFinding_HG_

