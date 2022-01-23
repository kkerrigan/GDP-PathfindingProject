/*
* Name:			cWait.h
* Author:		Daniel Maclam
* Date:			2018-12-10 7:42 AM
* Description:	
*/

#pragma once
#include "iCommand.h"

class cWait : public iCommand
{
	float duration;
	float timeElapsed;

	bool finished;

public:
	cWait(float duration);
	void update(float deltaTime) override;
	bool isFinished() override;
};

