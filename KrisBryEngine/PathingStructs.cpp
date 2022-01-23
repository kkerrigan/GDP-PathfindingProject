/**
 * @file PathingStructs.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementations for anying constructors/functions of structs in PathingStructs.h.
 */

#include "PathingStructs.h"

sPathFindingNode::sPathFindingNode() : neighbors{ nullptr } {

	this->position = glm::vec3(0.0f);
	this->parentNode = nullptr;
	this->bIsTraversable = false;
	this->bIsHomeBase = false;
	this->bIsGatherer = false;
	this->bIsResource = false;
	this->bInUse = false;

	this->currentDistance = 0.0f;
	this->currentDistanceFromNode = 0.0f;
	this->currentNodeWeight = 0.0f;

	// Set the self position in neighbor array
	this->neighbors[eNeighbor::SELF] = this;

	return;
}

sPathFindingNode* sPathFindingNode::getNeighbor(eNeighbor index) {
	
	return this->neighbors[index];
}

void sPathFindingNode::setNeighbor(eNeighbor index, sPathFindingNode* node) {

	this->neighbors[index] = node;

	return;
}
