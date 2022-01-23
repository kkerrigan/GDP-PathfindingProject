/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include "cThreadSystem.h"

#include <process.h>
#include <Windows.h>

CRITICAL_SECTION CSBufferIndex;

cThreadSystem::cThreadSystem() {

	InitializeCriticalSection(&CSBufferIndex);

	// We want the read buffer to be IN FRONT of the write buffer. 
	// So something like this:
	//
	// Buffer 0: write
	// Buffer 1: read
	// Buffer 2: -----
	// 
	// After switch:
	// Buffer 0: (dirty write)
	// Buffer 1: write 
	// Buffer 2: read
	// 
	// The "dirty write" is if some of the threads haven't 
	// realized that the buffers have switched. This is OK 
	// because there not chance of the reader READING bad
	// data, only that the occasional writer is out by one 
	// buffer value. 
	this->m_CurrentWriteBufferIndex = 0;
	this->m_CurrentReadBufferIndex = 1;

	return;
}


cThreadSystem::~cThreadSystem() {

	DeleteCriticalSection(&CSBufferIndex);

	return;
}

cThreadSystem* cThreadSystem::getInstance() {

	static cThreadSystem instance;

	return &instance;
}

void cThreadSystem::registerEntity(cEntity* entity) {

	this->mEntities.push_back(entity);

	return;
}

void cThreadSystem::unregisterEntity(cEntity*) {

	return;
}

void cThreadSystem::update(double deltaTime) {



	return;
}

void cThreadSystem::initBuffers(unsigned int size) {

	for (unsigned int buffIndex = 0; buffIndex != NUMBEROFBUFFERS; buffIndex++) {

		this->vecPositions[buffIndex].reserve(size);

		for (unsigned int posCount = 0; posCount != size; posCount++) {

			this->vecPositions[buffIndex].push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	return;
}

void cThreadSystem::switchBuffers(void) {

	EnterCriticalSection(&CSBufferIndex);

	this->m_CurrentWriteBufferIndex++;
	this->m_CurrentReadBufferIndex++;

	if (this->m_CurrentReadBufferIndex >= NUMBEROFBUFFERS) {
		this->m_CurrentReadBufferIndex = 0;
	}

	if (this->m_CurrentWriteBufferIndex >= NUMBEROFBUFFERS) {
		this->m_CurrentWriteBufferIndex = 0;
	}

	LeaveCriticalSection(&CSBufferIndex);
	return;
}

unsigned int cThreadSystem::getCurrentReadBuffer() {

	return this->m_CurrentReadBufferIndex;
}

unsigned int cThreadSystem::getCurrentWriteBuffer() {

	return this->m_CurrentWriteBufferIndex;
}