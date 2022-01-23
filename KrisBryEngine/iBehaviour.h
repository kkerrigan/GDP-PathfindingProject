/**
 * @file iBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * An interface that is implemented by each individual behaviour.
 */

#ifndef _iBehaviour_HG_
#define _iBehaviour_HG_

#include "cEntity.h"

class iBehaviour {
public:
	virtual ~iBehaviour() {}

	virtual void update(cEntity* pEntity, float deltaTime) = 0;
	virtual bool isFinished() = 0;
	virtual void startBehaviour() = 0;
	virtual void stopBehaviour() = 0;
	virtual bool isStarted() = 0;
};

#endif
