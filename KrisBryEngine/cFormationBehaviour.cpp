/**
 * @file cFormationBehaviour.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cFormationBehaviour class.
 */

#include <iostream>

#include "cFormationBehaviour.h"
#include "Common.h"
#include "cTransformComponent.h"

cFormationBehaviour::cFormationBehaviour() {

	this->mCurrentFormation = eFormationTypes::NONE;
	this->mCrowdNucleus = glm::vec3(0.0f);
	this->mCrowdDirection = FORWARD;
	this->mBIsBehaviourStarted = false;

	return;
}

cFormationBehaviour::~cFormationBehaviour() {

	return;
}

void cFormationBehaviour::registerEntity(cEntity* pEntity) {

	this->mFormationEntities.push_back(pEntity);

	return;
}

void cFormationBehaviour::unregisterEntity(cEntity* pEntity) {

	// TODO: Remove entity from formation
	return;
}

void cFormationBehaviour::update() {

	if (this->mCurrentFormation != eFormationTypes::NONE) {
		// Create the offsets for the current pattern
		std::vector<glm::vec3> formationOffsets = this->mGenerateOffsets(this->mCurrentFormation);

		// Set each entity in the formation to their new position
		for (size_t index = 0; index < this->mFormationEntities.size(); ++index) {

			cTransformComponent* pTransComp = this->mFormationEntities[index]->getComponent<cTransformComponent>();
			glm::vec3 position = pTransComp->getPosition();
			glm::vec3 currCheckpointPos = formationOffsets[index];
			float distToCurrCheckpoint = glm::distance(position, currCheckpointPos);

			glm::vec3 direction = glm::normalize(currCheckpointPos - position);
			glm::quat orientToCheckpoint = glm::quat(glm::inverse(
				glm::lookAt(position, position - direction, UP)));

			pTransComp->setQOrientation(orientToCheckpoint);
			glm::vec3 forwardVec = glm::toMat3(pTransComp->getQOrientation()) * FORWARD;

			// Near current checkpoint?
			if (distToCurrCheckpoint < 0.01f) continue;

			if (distToCurrCheckpoint < 0.15f) {

				pTransComp->setPosition(position + forwardVec * 0.01f);
			}
			else if (distToCurrCheckpoint < 30.0f) {

				pTransComp->setPosition(currCheckpointPos + forwardVec * 0.5f);
			}
			else {

				pTransComp->setPosition(position + forwardVec * 0.5f);
			}
		}
	}

	return;
}

void cFormationBehaviour::startBehaviour() {

	this->mBIsBehaviourStarted = true;

	return;
}

void cFormationBehaviour::stopBehaviour() {

	this->mBIsBehaviourStarted = false;

	return;
}

bool cFormationBehaviour::isStarted() {
	
	return this->mBIsBehaviourStarted;
}

void cFormationBehaviour::setFormation(eFormationTypes formation) {

	this->mCurrentFormation = formation;

	return;
}

glm::vec3 cFormationBehaviour::getCrowdNucleus() {

	return this->mCrowdNucleus;
}

void cFormationBehaviour::setCrowdNucleus(glm::vec3 newNucleus) {

	this->mCrowdNucleus = newNucleus;

	return;
}

void cFormationBehaviour::setCrowdDirection(glm::vec3 newDirection) {

	this->mCrowdDirection = newDirection;

	return;
}

std::vector<glm::vec3> cFormationBehaviour::mGenerateOffsets(eFormationTypes formation) {
	
	switch (formation) {
	case cFormationBehaviour::LINE:
		return this->mGenerateLineFormation();
		break;
	case cFormationBehaviour::SQUARE:
		return this->mGenerateSquareFormation();
		break;
	case cFormationBehaviour::CIRCLE:
		return this->mGenerateCircleFormation();
		break;
	case cFormationBehaviour::VSHAPE:
		return this->mGenerateVShapeFormation();
		break;
	case cFormationBehaviour::TWO_ROWS:
		return this->mGenerateTwoRowsFormation();
		break;
	case cFormationBehaviour::NONE:
		std::cout << "No formation is currently running!" << std::endl;
		break;
	default:
		std::cout << "No eFormationTypes was set for this group!" << std::endl;
		break;
	}
}

std::vector<glm::vec3> cFormationBehaviour::mGenerateLineFormation() {
	
	// Get the number of entities and the center point
	size_t formationSize = this->mFormationEntities.size();
	size_t center = formationSize / 2;

	// Set a value to space each member out
	// TODO: Make this configurable
	glm::mat4 identity = glm::mat4(1.0f);
	glm::vec3 leftEnd = glm::normalize(glm::cross(this->mCrowdDirection, UP));
	glm::vec3 personalSpace = glm::vec3(4.0f, 4.0f, 4.0f) * leftEnd;

	// The start of the offset will be the center point multiplied by negative personalSpace
	// (line goes left to right) // TODO: make the right to left togglable
	glm::vec3 startingOffset = this->mCrowdNucleus + static_cast<float>(center) * -personalSpace;
	std::vector<glm::vec3> formation;
	for (size_t index = 0; index < formationSize; ++index) {

		formation.push_back(startingOffset);
		startingOffset += personalSpace;
	}

	return formation;

}

std::vector<glm::vec3> cFormationBehaviour::mGenerateSquareFormation() {
	
	// Get the total formation size, the center, and half the center
	// The half of the center determines where the second row of the square begins
	size_t formationSize = this->mFormationEntities.size();
	size_t centre = formationSize / 2;
	size_t rowSplit = centre / 2;

	// This formation needs a spacing in both the X and Z axis
	glm::vec3 leftEnd = glm::normalize(glm::cross(this->mCrowdDirection, UP));
	glm::vec3 personalSpaceX = glm::vec3(4.0f, 4.0f, 4.0f) * leftEnd;
	glm::vec3 personalSpaceZ = glm::vec3(-4.0f, -4.0f, -4.0f) * this->mCrowdDirection;

	glm::vec3 startingOffset = this->mCrowdNucleus + static_cast<float>(rowSplit) * -personalSpaceX;
	std::vector<glm::vec3> formation;
	for (size_t outer = 0; outer < 4; ++outer) {

		for (size_t index = 0; index < rowSplit; ++index) {

			formation.push_back(startingOffset);
			startingOffset += personalSpaceX;
		}

		startingOffset += static_cast<float>(rowSplit) * -personalSpaceX;
		startingOffset += personalSpaceZ;
	}

	return formation;
}

std::vector<glm::vec3> cFormationBehaviour::mGenerateCircleFormation() {
	
	size_t formationSize = this->mFormationEntities.size();

	std::vector<glm::vec3> formation;
	float rotation = 2.0f * glm::pi<float>() / formationSize;

	for (size_t index = 0; index < formationSize; ++index) {

		float circleRotation = index * rotation;
		float pointX = this->mCrowdNucleus.x + cos(circleRotation) * 10.0f;
		float pointZ = this->mCrowdNucleus.z + sin(circleRotation) * 10.0f;

		formation.push_back(glm::vec3(pointX, 0.0f, pointZ));
	}

	return formation;
}

std::vector<glm::vec3> cFormationBehaviour::mGenerateVShapeFormation() {

	size_t formationSize = this->mFormationEntities.size();
	size_t centre = formationSize / 2;

	// For the V-Shape the starting offset should be the crowd nucleus
	glm::vec3 startingOffset(this->mCrowdNucleus);

	glm::vec3 leftEnd = glm::normalize(glm::cross(this->mCrowdDirection, UP));
	glm::vec3 personalSpaceX = glm::vec3(4.0f, 4.0f, 4.0f) * leftEnd;
	glm::vec3 personalSpaceZ = glm::vec3(-4.0f, -4.0f, -4.0f) * this->mCrowdDirection;

	std::vector<glm::vec3> formation;
	formation.push_back(startingOffset);
	for (size_t index = 1; index < formationSize; index += 2) {

		glm::vec3 leftArm = startingOffset - (personalSpaceX * static_cast<float>(index)) + (personalSpaceZ * static_cast<float>(index));
		glm::vec3 rightArm = startingOffset + (personalSpaceX * static_cast<float>(index)) + (personalSpaceZ * static_cast<float>(index));

		formation.push_back(leftArm);
		formation.push_back(rightArm);
	}

	return formation;
}

std::vector<glm::vec3> cFormationBehaviour::mGenerateTwoRowsFormation() {
	
	// Get the total formation size, the center, and half the center
	// The half of the center determines where the second row begins
	size_t formationSize = this->mFormationEntities.size();
	size_t centre = formationSize / 2;
	size_t rowSplit = centre / 2;

	// This formation needs a spacing in both the X and Z axis
	glm::vec3 leftEnd = glm::normalize(glm::cross(this->mCrowdDirection, UP));
	glm::vec3 personalSpaceX = glm::vec3(5.0f, 5.0f, 5.0f) * leftEnd;
	glm::vec3 personalSpaceZ = glm::vec3(-5.0f, -5.0f, -5.0f) * this->mCrowdDirection;

	// The starting offset will first be the beginning of the row split multiplied by negative X spacing
	glm::vec3 startingOffset = this->mCrowdNucleus + static_cast<float>(rowSplit) * -personalSpaceX;
	std::vector<glm::vec3> formation;
	for (size_t index = 0; index < centre; ++index) {

		formation.push_back(startingOffset);
		startingOffset += personalSpaceX;
	}

	// Set the starting offset back to its original value and also add Z spacing
	startingOffset = this->mCrowdNucleus + static_cast<float>(rowSplit) * -personalSpaceX;
	startingOffset += personalSpaceZ;
	for (size_t index = centre; index < formationSize; ++index) {

		formation.push_back(startingOffset);
		startingOffset += personalSpaceX;
	}

	return formation;
}
