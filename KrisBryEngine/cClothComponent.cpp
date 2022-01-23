/**
 * @file cClothComponent.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cClothComponent class.
 */

#include "cClothComponent.h"

cClothComponent::cClothComponent() : cComponent() {

	this->mType = "Cloth";
	this->mSoftBody = nullptr;
}

cClothComponent::~cClothComponent() {

	delete this->mSoftBody;
	this->mSoftBody = nullptr;

	return;
}

nPhysics::iSoftBody* cClothComponent::getSoftBody() {

	return this->mSoftBody;
}

void cClothComponent::setSoftBody(nPhysics::iSoftBody* softBody) {

	this->mSoftBody = softBody;
}