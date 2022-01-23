/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cInstancedMeshComponentTEST.h"

cInstancedMeshComponent::cInstancedMeshComponent() : cComponent() {


	return;
}

cInstancedMeshComponent::~cInstancedMeshComponent() {

	return;
}

unsigned int cInstancedMeshComponent::getAmount() {
	
	return this->mAmount;
}

void cInstancedMeshComponent::setAmount(unsigned int amount) {

	this->mAmount = amount;
}
