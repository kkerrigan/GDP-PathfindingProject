#include "cFrameBufferComponent.h"
#include <string>
#include <iostream>

cFrameBufferComponent::cFrameBufferComponent() : cComponent() {

	this->pFbo = nullptr;

	return;
}

cFrameBufferComponent::~cFrameBufferComponent() {

	return;
}

void cFrameBufferComponent::inititFrameBuffer(int texNumber) {

	this->mFboTextureUnit = texNumber;

	// Create a new cFBO object
	this->pFbo = new cFBO();

	// Try to initialize the FBO
	std::string errorString;
	if (this->pFbo->init(1024, 720, errorString)) {

		std::cout << "Frame Buffer good to go!" << std::endl;
	}
	else {
		
		std::cout << "One of your frame buffers failed to initialize!" << std::endl;
	}

	return;
}

int cFrameBufferComponent::getFboTextureUnit() const {
	
	return this->mFboTextureUnit;
}
