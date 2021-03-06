#include "cRandThreaded.h"
#include <cstdlib>	// RAND_MAX
#include <Windows.h>	// Critical Sections


cRandThreaded::cRandThreaded() {
	
	InitializeCriticalSection( &(this->CS_bufferIndexLock) );

	// Load initial list of random numbmers
	this->m_LoadBufferWithRandoms();

	return;
}

cRandThreaded::~cRandThreaded() {
	
	DeleteCriticalSection( &(this->CS_bufferIndexLock ) );

	return;
}


double cRandThreaded::getNextRandDouble_0_to_1(void) {
	
	EnterCriticalSection(&(this->CS_bufferIndexLock));

	this->lastReadIndex++;

	if (this->lastReadIndex >= cRandThreaded::SIZEOFBUFFER) {
		
		this->m_LoadBufferWithRandoms();
		this->lastReadIndex = 0;
	}

	double theRandomNumber = this->randDoubles[this->lastReadIndex];

	LeaveCriticalSection(&(this->CS_bufferIndexLock ) );

	return theRandomNumber;
};


double cRandThreaded::getNextRandDoubleInRange(double min, double max) {
	
	// Value is between 0.0 and 1.0 in the array
	double value = this->getNextRandDouble_0_to_1();

	value = min + value*(max-min); 

	return value;
}

void cRandThreaded::m_LoadBufferWithRandoms(void) {
	
	for (int index = 0; index != cRandThreaded::SIZEOFBUFFER; index++) {
		
		this->randDoubles[index] = getRandInRange_RT<double>(0.0, 1.0);
	}
	
	this->lastReadIndex = 0;

	return;
}

