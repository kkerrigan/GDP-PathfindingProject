/**
 * @file iCrowdBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This is an interface class that defines the behaviour of groups of entities (crowds) with 
 * specific behaviours.
 */

#ifndef _iCrowdBehaviour_HG_
#define _iCrowdBehaviour_HG_

#include "cEntity.h"

class iCrowdBehaviour {
public:
	virtual ~iCrowdBehaviour() {}

	virtual void registerEntity(cEntity* pEntity) = 0;
	virtual void unregisterEntity(cEntity* pEntity) = 0;
	virtual void update() = 0;
	virtual void startBehaviour() = 0;
	virtual void stopBehaviour() = 0;
	virtual bool isStarted() = 0;
};

#endif // !_iCrowdBehaviour_HG_

