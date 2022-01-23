/*
* Name:		cWait.h
* Author:	Daniel Maclam
* Date		2018-12-10 7:42 AM
*/

#include "cWait.h"

cWait::cWait(float duration) : duration(duration), timeElapsed(0), finished(false)
{
}

void cWait::update(float deltaTime)
{
	this->timeElapsed += deltaTime;

	if(this->timeElapsed >= this->duration)
	{
		this->finished = true;
	}
}

bool cWait::isFinished()
{
	return this->finished;
}
