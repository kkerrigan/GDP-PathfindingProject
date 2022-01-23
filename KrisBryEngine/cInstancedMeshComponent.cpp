/**
 * @file cInstancedMeshComponent.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cInstancedMeshComponent class.
 */

#include "cInstancedMeshComponent.h"

cInstancedMeshComponent::cInstancedMeshComponent() : cComponent() {


	return;
}

cInstancedMeshComponent::~cInstancedMeshComponent() {

	return;
}

/**
* generateDrawInfo
* @brief This function creates the draw info groups for the different map tile types.
* @return A map<string, sDrawInfo> that represents the number and positions of each tile type.
*/
std::map<std::string, cInstancedMeshComponent::sDrawInfo> cInstancedMeshComponent::generateDrawInfo() {
	
	std::map<std::string, sDrawInfo> drawInfo;

	for (size_t index = 0; index < this->instances.size(); ++index) {

		glm::vec4 currColour = this->instances[index].color;

		if (currColour.r == 0.0f && currColour.g == 0.0f && currColour.b == 0.0f) {
			
			drawInfo["black"].instances.push_back(&this->instances[index]);
			drawInfo["black"].numInstances++;
		}

		if (currColour.r == 1.0f && currColour.g == 0.0f && currColour.b == 0.0f) {
			
			drawInfo["red"].instances.push_back(&this->instances[index]);
			drawInfo["red"].numInstances++;
		}

		if (currColour.r == 0.0f && currColour.g == 1.0f && currColour.b == 0.0f) {
			
			drawInfo["green"].instances.push_back(&this->instances[index]);
			drawInfo["green"].numInstances++;
		}

		if (currColour.r == 0.0f && currColour.g == 0.0f && currColour.b == 1.0f) {
			
			drawInfo["blue"].instances.push_back(&this->instances[index]);
			drawInfo["blue"].numInstances++;
		}

		if (currColour.r == 1.0f && currColour.g == 1.0f && currColour.b == 1.0f) {
			
			drawInfo["white"].instances.push_back(&this->instances[index]);
			drawInfo["white"].numInstances++;
		}
	}

	return drawInfo;
}

/**
* updateInstances
* @brief This function updates the instances of the component.
* @param vector<sPathFindingNode*> nodes - represents the nodes for the path finding.
* @param bool renderTraversable - represents whether or not to display the empty spaces (Optional)
* @return void.
*/
void cInstancedMeshComponent::updateInstances(std::vector<sPathFindingNode*> nodes, bool renderTraversable) {

	instances.clear();

	for (size_t i = 0; i < nodes.size(); ++i) {
		
		sMesh newMesh;
		newMesh.position = nodes[i]->position;
		newMesh.position.y -= 0.98f;
		
		if (!nodes[i]->bIsTraversable) {
			
			newMesh.position.y += 2.0f; // Make the walls of the maze slightly raised
			newMesh.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsResource) {
			
			newMesh.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsGatherer) {
			
			newMesh.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (nodes[i]->bIsHomeBase) {
			
			newMesh.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			this->instances.push_back(newMesh);
		}

		if (renderTraversable && nodes[i]->bIsTraversable) {
			
			newMesh.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			this->instances.push_back(newMesh);
		}
	}

	return;
}
