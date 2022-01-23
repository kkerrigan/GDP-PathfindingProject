/**
 * @file cPathFinding.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cPathFinding class.
 */

#include <iostream>
#include <algorithm>

#include "cPathFinding.h"

 /**
 * findShortestDijkstraPath
 * @brief Uses cDijkstra's algorithm to find the shortest path.
 * @param glm::vec3 currentPosition - The current position of the agent.
 * @param sPathFindingNode* startNode - Where to begin the pathfinding algorithm.
 * @param sPathFindingNode* goalNode - The destination for the shortest path.
 * @return A sPathFinding representing the positions for the shortest path.
 */
sPathFinding cPathFinding::findShortestDijkstraPath(glm::vec3 currentPosition, sPathFindingNode* startNode, sPathFindingNode* goalNode) {

	// First create the open and close set of nodes.
	std::vector<sPathFindingNode*> openSet;
	std::vector<sPathFindingNode*> closedSet;

	// Set all the nodes distance to FLT_MAX
	for (size_t index = 0; index < this->path.pathNodes.size(); ++index) {

		this->path.pathNodes[index]->parentNode = nullptr;
		this->path.pathNodes[index]->currentDistance = FLT_MAX;
	}

	// Set the distance to the startNode to 0.0f
	startNode->currentDistance = 0.0f;

	// Add the start node to the open set and then calculate the path.
	this->mAddToOpenSet(openSet, startNode);

	return this->mCalculateDijkstraPath(openSet, closedSet, goalNode);
}

/**
 * findClosestNodeType
 * @brief Uses cDijkstra's algorithm to find the closest tile type.
 * @param glm::vec3 startPosition - The position to start looking for a tile.
 * @param cMapLoader::eTileType tileType - The type of tile to search for.
 * @return A sPathFindingNode* representing the shortest path.
 */
sPathFindingNode* cPathFinding::findClosestTileType(glm::vec3 startPosition, cMapLoader::eTileType tileType) {

	// First create the open and close set of nodes.
	std::vector<sPathFindingNode*> openSet;
	std::vector<sPathFindingNode*> closedSet;

	// Set all the nodes distance to FLT_MAX
	for (size_t index = 0; index < this->path.pathNodes.size(); ++index) {

		this->path.pathNodes[index]->parentNode = nullptr;
		this->path.pathNodes[index]->currentDistance = FLT_MAX;
	}

	sPathFindingNode* startNode = this->findClosestNode(startPosition);

	// Set the distance to the startNode to 0.0f and add it to the open set
	startNode->currentDistance = 0.0f;
	openSet.push_back(startNode);

	sPathFindingNode* goalNode = nullptr;

	while (openSet.size() != 0) {

		// Remove the lowest cost node
		int maxIndex = 0;
		float maxDistance = FLT_MAX;
		for (size_t index = 0; index < openSet.size(); ++index) {

			if (openSet[index]->currentDistance < maxDistance) {

				maxDistance = openSet[index]->currentDistance;
				maxIndex = index;
			}
		}

		sPathFindingNode* closestNode = openSet[maxIndex];
		this->mRemoveFromOpenSet(openSet, closestNode);

		// Add closestNode to the closed set
		this->mAddToClosedSet(closedSet, closestNode);

		// Determine the goalNode based on the tile type we are searching for
		switch (tileType) {
		case cMapLoader::HOME_BASE:
			if (closestNode->bIsHomeBase) {

				goalNode = closestNode;
				openSet.clear();
			}
			break;
		case cMapLoader::RESOURCE:
			if (closestNode->bIsResource && !closestNode->bInUse) {

				goalNode = closestNode;
				openSet.clear();
			}
			break;
		case cMapLoader::UNKNOWN:
			break;
		default:
			break;
		}

		// Check if we are at the goal
		if (goalNode) {

			openSet.clear();
			return goalNode;
		}

		// Add neighbors to the open set
		for (size_t index = 0; index < 9; ++index) {

			sPathFindingNode* currNeighbor = closestNode->neighbors[index];

			if (currNeighbor && currNeighbor->bIsTraversable) {

				// check if this node is already in the open set
				if (this->mIsNodeInSet(openSet, currNeighbor)) {

					// Check for a faster route
					float newDistance = closestNode->currentDistance + glm::distance(currNeighbor->position, closestNode->position);
					if (currNeighbor->currentDistance > newDistance) {

						currNeighbor->currentDistance = newDistance;
						currNeighbor->parentNode = closestNode;
					}

					continue;
				}

				// Check if the current neighbor is already in the closed set
				if (this->mIsNodeInSet(closedSet, currNeighbor)) continue;

				// If a diagonal is blocked, don't go that way
				if (index == eNeighbor::TOP_LEFT || index == eNeighbor::TOP_RIGHT || index == eNeighbor::BOTTOM_LEFT || index == eNeighbor::BOTTOM_RIGHT) {

					if (!this->mIsValidDiagonal(closestNode, index)) continue;
				}

				// Add the current neighbor to the open set
				this->mAddToOpenSet(openSet, currNeighbor);

				// Set the parent for the neighbor
				currNeighbor->parentNode = closestNode;

				// Calculate the weight for this neighbor
				currNeighbor->currentDistance = closestNode->currentDistance + glm::distance(currNeighbor->position, closestNode->position);
			}
		}
	} // while (openSet.size() != 0)

	if (!goalNode) {

		return nullptr;
	}
}

/**
 * findShortestAStarPath
 * @brief Uses A* algorithm to find the shortest path.
 * @param glm::vec3 currentPosition - The current position of the agent.
 * @param sPathFindingNode* startNode - Where to begin the pathfinding algorithm.
 * @param sPathFindingNode* goalNode - The destination for the shortest path.
 * @return A sPathFinding representing the positions for the shortest path.
 */
sPathFinding cPathFinding::findShortestAStarPath(glm::vec3 currentPosition, sPathFindingNode* startNode, sPathFindingNode* goalNode) {

	// Create the open and closed set
	std::vector<sPathFindingNode*> openSet;
	std::vector<sPathFindingNode*> closedSet;

	// Set all nodes current distance to FLT_MAX and the parent node to null
	for (size_t index = 0; index < this->path.pathNodes.size(); ++index) {

		this->path.pathNodes[index]->currentDistance = FLT_MAX;
		this->path.pathNodes[index]->parentNode = nullptr;
	}

	// Set the start node distance to zero
	startNode->currentDistance = 0.0f;

	// Add the start node to the open set and calculate the path
	this->mAddToOpenSet(openSet, startNode);
	return this->mCalculateAStarPath(openSet, closedSet, goalNode);
}

/**
* findClosestNode
* @brief Find the closest node to your location.
* @param glm::vec3 position - Your current position.
* @return A sPathFindingNode* representing the closest node.
*/
sPathFindingNode* cPathFinding::findClosestNode(glm::vec3 position) {

	float maxDistance = FLT_MAX;
	unsigned long long maxIndex = 0;

	for (unsigned long long index = 0; index < this->path.pathNodes.size(); ++index) {

		float currentDistance = glm::abs(glm::distance(this->path.pathNodes[index]->position, position));
		if (currentDistance < maxDistance) {

			maxDistance = currentDistance;
			maxIndex = index;
		}
	}

	return this->path.pathNodes[maxIndex];
}

/**
* findGathererNodes
* @brief Find all the gatherers on the map.
* @return A sPathFinding (vector of nodes) representing the gatherers.
*/
sPathFinding cPathFinding::findGathererNodes() {

	sPathFinding gatherers;
	for (size_t index = 0; index < this->path.pathNodes.size(); ++index) {

		if (this->path.pathNodes[index]->bIsGatherer) {

			gatherers.pathNodes.push_back(this->path.pathNodes[index]);
		}
	}

	return gatherers;
}

/**
* findResourceNodes
* @brief Find all the resources on the map.
* @param Add any params
* @return A sPathFinding (vector of nodes) representing the resources.
*/
sPathFinding cPathFinding::findResourceNodes() {

	sPathFinding resources;
	for (size_t index = 0; index < this->path.pathNodes.size(); ++index) {

		if (this->path.pathNodes[index]->bIsResource) {

			resources.pathNodes.push_back(this->path.pathNodes[index]);
		}
	}

	return resources;
}

/**
* findNodeNeighbors
* @brief Finds the neighbours for each node in the path.
* @param unsigned int mapWidth - the width of the path finding area.
* @param unsigned int mapHeight - the height of the path finding area.
* @return void
*/
void cPathFinding::findNodeNeighbors(unsigned int mapWidth, unsigned int mapHeight) {

	// Check that the nodes have been created for the path
	if (this->path.pathNodes.empty()) {

		std::cerr << "The nodes of the path must be created before the neighbors can be found!" << std::endl;
		return;
	}

	int index = 0;
	for (size_t x = 0; x < mapHeight; x++) {

		for (size_t y = 0; y < mapWidth; y++) {

			// Top-Left
			if (y > 0 && x > 0) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::TOP_LEFT, this->path.pathNodes[index - mapWidth - 1]);
			}

			// Top-Centre
			if (x > 0) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::TOP_CENTRE, this->path.pathNodes[index - mapWidth]);
			}

			// Top-Right
			if (y < mapWidth - 1 && x > 0) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::TOP_RIGHT, this->path.pathNodes[index - mapWidth + 1]);
			}

			//left
			if (y > 0) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::LEFT, this->path.pathNodes[index - 1]);
			}

			//right
			if (y < mapWidth - 1) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::RIGHT, this->path.pathNodes[index + 1]);
			}

			// Bottom-Left
			if (y > 0 && x < mapHeight - 1) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::BOTTOM_LEFT, this->path.pathNodes[index + mapWidth - 1]);
			}

			// Bottom-Centre
			if (x < mapHeight - 1) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::BOTTOM_CENTRE, this->path.pathNodes[index + mapWidth]);
			}

			// Bottom-Right
			if (y < mapWidth - 1 && x < mapHeight - 1) {

				this->path.pathNodes[index]->setNeighbor(eNeighbor::BOTTOM_RIGHT, this->path.pathNodes[index + mapWidth + 1]);
			}

			index++;
		}
	}

	return;
}

/**
* mAddToOpenSet
* @brief Adds a path node to the open set of the algorithm.
* @param std::vector<sPathFindingNode*>& openSet - The current open set.
* @param sPathFindingNode* node - The path node to be added.
* @return void.
*/
void cPathFinding::mAddToOpenSet(std::vector<sPathFindingNode*>& openSet, sPathFindingNode* node) {

	// Check to see if it has already been added
	std::vector<sPathFindingNode*>::iterator it = std::find(openSet.begin(), openSet.end(), node);
	if (it != openSet.end()) return; // Already in the open set

	openSet.push_back(node);

	return;
}

/**
* mRemoveFromOpenSet
* @brief Removes a path node from the open set of the algorithm.
* @param std::vector<sPathFindingNode*>& openSet - The current open set.
* @param sPathFindingNode* node - The path node to be removed.
* @return A bool representing whether the removal was successful.
*/
bool cPathFinding::mRemoveFromOpenSet(std::vector<sPathFindingNode*>& openSet, sPathFindingNode* node) {

	std::vector<sPathFindingNode*>::iterator it = std::remove(openSet.begin(), openSet.end(), node);
	if (it == openSet.end()) return false;

	openSet.erase(it, openSet.end());

	return true;
}

/**
* mAddToClosedSet
* @brief Adds a path node to the closed set of the algorithm.
* @param std::vector<sPathFindingNode*>& closeSet - The current close set.
* @param sPathFindingNode* node - The path node to be added.
* @return void.
*/
void cPathFinding::mAddToClosedSet(std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* node) {

	// Check to see if it has already been added
	std::vector<sPathFindingNode*>::iterator it = std::find(closedSet.begin(), closedSet.end(), node);
	if (it != closedSet.end()) return; // Already in the open set

	closedSet.push_back(node);

	return;
}

/**
* mRemoveFromClosedSet
* @brief Removes a path node from the close set of the algorithm.
* @param std::vector<sPathFindingNode*>& closeSet - The current close set.
* @param sPathFindingNode* node - The path node to be removed.
* @return A bool representing whether the removal was successful.
*/
bool cPathFinding::mRemoveFromClosedSet(std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* node) {

	std::vector<sPathFindingNode*>::iterator it = std::remove(closedSet.begin(), closedSet.end(), node);
	if (it == closedSet.end()) return false;

	closedSet.erase(it, closedSet.end());

	return true;
}

/**
* mCalculateDijkstraPath
* @brief Performs Dijkstra's algorithm to find the path.
* @param std::vector<sPathFindingNode*>& openSet - The open set of nodes.
* @param std::vector<sPathFindingNode*>& closedSet - The closed set of nodes.
* @param sPathFindingNode* goalNode - The path end point.
* @return sPathFinding representing the path.
*/
int dijkstraCount = 0;
sPathFinding cPathFinding::mCalculateDijkstraPath(std::vector<sPathFindingNode*>& openSet, std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* goalNode) {

	while (openSet.size() != 0) {

		// Increment the number of nodes checked
		dijkstraCount++;

		// Remove the lowest cost node
		int maxIndex = 0;
		float maxDistance = FLT_MAX;
		for (size_t index = 0; index < openSet.size(); ++index) {

			if (openSet[index]->currentDistance < maxDistance) {

				maxDistance = openSet[index]->currentDistance;
				maxIndex = index;
			}
		}

		sPathFindingNode* closestNode = openSet[maxIndex];
		this->mRemoveFromOpenSet(openSet, closestNode);

		// Add closestNode to the closed set
		this->mAddToClosedSet(closedSet, closestNode);

		// Check if we are at the goal
		if (closestNode == goalNode) {

			openSet.clear();
		}

		// Add neighbors to the open set
		for (size_t index = 0; index < 9; ++index) {

			sPathFindingNode* currNeighbor = closestNode->neighbors[index];

			if (currNeighbor && currNeighbor->bIsTraversable) {

				// check if this node is already in the open set
				if (this->mIsNodeInSet(openSet, currNeighbor)) {

					// Check for a faster route
					float newDistance = closestNode->currentDistance + glm::distance(currNeighbor->position, closestNode->position);
					if (currNeighbor->currentDistance > newDistance) {

						currNeighbor->currentDistance = newDistance;
						currNeighbor->parentNode = closestNode;
					}

					continue;
				}

				// Check if the current neighbor is already in the closed set
				if (this->mIsNodeInSet(closedSet, currNeighbor)) continue;

				// If a diagonal is blocked, don't go that way
				if (index == eNeighbor::TOP_LEFT || index == eNeighbor::TOP_RIGHT || index == eNeighbor::BOTTOM_LEFT || index == eNeighbor::BOTTOM_RIGHT) {

					if (!this->mIsValidDiagonal(closestNode, index)) continue;
				}

				// Add the current neighbor to the open set
				this->mAddToOpenSet(openSet, currNeighbor);

				// Set the parent for the neighbor
				currNeighbor->parentNode = closestNode;

				// Calculate the weight for this neighbor
				currNeighbor->currentDistance = closestNode->currentDistance + glm::distance(currNeighbor->position, closestNode->position);
			}
		}
	} // while (openSet.size() != 0)

	// Print out how many nodes were checked
	printf("Dijkstra checked %d nodes.\n", dijkstraCount);
	dijkstraCount = 0;

	sPathFinding shortestPath;
	sPathFindingNode* currentNode = goalNode;
	while (currentNode != nullptr) {

		shortestPath.pathPositions.push_back(currentNode->position);
		currentNode = currentNode->parentNode;
	}

	// The vector was filled backwards so reverse it
	std::reverse(shortestPath.pathPositions.begin(), shortestPath.pathPositions.end());

	return shortestPath;
}

/**
* mCalculateAStarPath
* @brief Performs A* algorithm to find the path.
* @param std::vector<sPathFindingNode*>& openSet - The open set of nodes.
* @param std::vector<sPathFindingNode*>& closedSet - The closed set of nodes.
* @param sPathFindingNode* goalNode - The path end point.
* @return sPathFinding representing the path.
*/
int aStarCount = 0;
sPathFinding cPathFinding::mCalculateAStarPath(std::vector<sPathFindingNode*>& openSet, std::vector<sPathFindingNode*>& closedSet, sPathFindingNode* goalNode) {

	while (openSet.size() != 0) {

		// Increment the number of nodes checked
		aStarCount++;

		//remove lowest cost node using euclidean hueristic
		int maxIndex = 0;
		float maxDistance = FLT_MAX;
		for (size_t index = 0; index < openSet.size(); ++index) {

			if (openSet[index]->currentNodeWeight < maxDistance) {

				maxDistance = openSet[index]->currentNodeWeight;
				maxIndex = index;
			}
		}

		sPathFindingNode* closestNode = openSet[maxIndex];
		this->mRemoveFromOpenSet(openSet, closestNode);

		// Add closest node to the closed set
		this->mAddToClosedSet(closedSet, closestNode);

		// Check if we are at the goal
		if (closestNode == goalNode) {

			openSet.clear();
			break;
		}

		// Add the neighbors to the closed set
		for (size_t index = 0; index < 9; ++index) {

			sPathFindingNode* currNeighbor = closestNode->neighbors[index];

			if (currNeighbor && currNeighbor->bIsTraversable) {

				if (this->mIsNodeInSet(openSet, currNeighbor)) {

					currNeighbor->currentDistanceFromNode = this->mCalculateEuclideanHeuristic(currNeighbor, goalNode);

					// Check if this is a faster route
					float newDistance = closestNode->currentDistance + glm::distance(currNeighbor->position, closestNode->position);
					if (closestNode->currentNodeWeight > newDistance) {

						currNeighbor->currentDistance = newDistance;
						currNeighbor->parentNode = closestNode;
					}

					continue;
				}

				// If the current neighbor is in the closed set, continue
				if (this->mIsNodeInSet(closedSet, currNeighbor)) continue;

				// If a diagonal is blocked, don't go that way
				if (index == eNeighbor::TOP_LEFT || index == eNeighbor::TOP_RIGHT || index == eNeighbor::BOTTOM_LEFT || index == eNeighbor::BOTTOM_RIGHT) {

					if (!this->mIsValidDiagonal(closestNode, index)) continue;
				}

				// Add current neighbor to open set and set parent node
				this->mAddToOpenSet(openSet, currNeighbor);
				currNeighbor->parentNode = closestNode;

				// Finally calculate the euclidean hueristic weights
				currNeighbor->currentDistanceFromNode = this->mCalculateEuclideanHeuristic(currNeighbor, goalNode);
				currNeighbor->currentDistance = closestNode->currentDistance + glm::abs(glm::distance(currNeighbor->position, closestNode->position));
				currNeighbor->currentNodeWeight = currNeighbor->currentDistance + glm::pow(currNeighbor->currentDistanceFromNode, 2);
			}
		}
	} // while (openSet.size() != 0)

	// Print out how many nodes were checked
	printf("A* checked %d nodes.\n", aStarCount);
	aStarCount = 0;

	sPathFinding shortestPath;
	sPathFindingNode* currentNode = goalNode;
	while (currentNode != nullptr) {

		shortestPath.pathPositions.push_back(currentNode->position);
		currentNode = currentNode->parentNode;
	}

	// The vector was filled backwards so reverse it
	std::reverse(shortestPath.pathPositions.begin(), shortestPath.pathPositions.end());

	return shortestPath;
}

/**
* mCalculateEuclideanHeuristic
* @brief Calculates the Euclidean heuristic value for the current node to the goal.
* @param sPathFindingNode* currentNode - The node being tested.
* @param sPathFindingNode* goalNode - The destination node.
* @return A float value representing the heuristic for the current node.
*/
float cPathFinding::mCalculateEuclideanHeuristic(sPathFindingNode* currentNode, sPathFindingNode* goalNode) {

	float dx = abs(currentNode->position.x - goalNode->position.x);
	float dy = abs(currentNode->position.z - goalNode->position.z);

	return sqrt(dx * dx + dy * dy);
}

/**
* mIsNodeInSet
* @brief Checks to see if a node is in the given set.
* @param std::vector<sPathFindingNode*>& set - The set to be checked.
* @param sPathFindingNode* node - The node to look for.
* @return A bool representing whether the node was present or not.
*/
bool cPathFinding::mIsNodeInSet(std::vector<sPathFindingNode*>& set, sPathFindingNode* node) {

	// Check to see if it has already been added
	std::vector<sPathFindingNode*>::iterator it = std::find(set.begin(), set.end(), node);
	if (it == set.end()) return false; // Node wasn't found

	return true;
}

/**
* mIsValidDiagonal
* @brief Checks to see if the closest node is able to travel in a given diagonal path.
* @param sPathFindingNode* closestNode - The node being checked.
* @param int diagonalIndex - The diagonal position.
* @return A bool representing whether the diagonal path is valid.
*/
bool cPathFinding::mIsValidDiagonal(sPathFindingNode* closestNode, int diagonalIndex) {

	if (diagonalIndex == eNeighbor::TOP_LEFT) {

		return closestNode->neighbors[eNeighbor::LEFT]->bIsTraversable && closestNode->neighbors[eNeighbor::TOP_CENTRE]->bIsTraversable;
	}

	if (diagonalIndex == eNeighbor::TOP_RIGHT) {

		return closestNode->neighbors[eNeighbor::RIGHT]->bIsTraversable && closestNode->neighbors[eNeighbor::TOP_CENTRE]->bIsTraversable;
	}

	if (diagonalIndex == eNeighbor::BOTTOM_LEFT) {

		return closestNode->neighbors[eNeighbor::LEFT]->bIsTraversable && closestNode->neighbors[eNeighbor::BOTTOM_CENTRE]->bIsTraversable;
	}

	if (diagonalIndex == eNeighbor::BOTTOM_RIGHT) {

		return closestNode->neighbors[eNeighbor::RIGHT]->bIsTraversable && closestNode->neighbors[eNeighbor::BOTTOM_CENTRE]->bIsTraversable;
	}

	return false; // Should never get here
}
