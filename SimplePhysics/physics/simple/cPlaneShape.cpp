/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cPlaneShape.h"

nPhysics::cPlaneShape::cPlaneShape(const glm::vec3& normal, const float constant) {

	this->mNormal = normal;
	this->mConstant = constant;

	return;
}

float nPhysics::cPlaneShape::getPlaneConstant() {
	
	return this->mConstant;
}

glm::vec3 nPhysics::cPlaneShape::getPlaneNormal() {
	
	return this->mNormal;
}
