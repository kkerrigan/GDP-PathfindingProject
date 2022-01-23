/**
 * @file cPatrolBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will cause the entity to do one of 2 things:
		1. Wander to a random location within the map
		2. Idle at that location for a specified time
	It will then repeat with a new location.
 */

#ifndef _cPatrolBehaviour_HG_
#define _cPatrolBehaviour_HG_

#include <gameMath.h>

#include "iBehaviour.h"
#include "cWanderBehaviour.h"
#include "cIdleBehaviour.h"
#include "cEntity.h"

class cPatrolBehaviour : public iBehaviour {
public:
	cPatrolBehaviour(cEntity* agent, float timeBetweenWander, float mapExtents);
	virtual ~cPatrolBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;

private:
	cEntity* mAgent;
	float mTimeBetweenWanders;
	float mMapExtents;

	cWanderBehaviour* mWander;
	cIdleBehaviour* mIdle;
	bool mIsIdle;
};

#endif // !_cPatrolBehaviour_HG_

