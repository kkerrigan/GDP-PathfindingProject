/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cSphereShape.h"

nPhysics::cSphereShape::cSphereShape(const float radius) {

	this->mRadius = radius;

	return;
}

float nPhysics::cSphereShape::getRadius() {
	
	return this->mRadius;
}
