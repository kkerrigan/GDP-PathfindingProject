/**
 * @file cWanderBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will cause the entity to wander to different positions within a given radius.
 */

#ifndef _cWanderBehaviour_HG_
#define _cWanderBehaviour_HG_

#include <gameMath.h>

#include "iBehaviour.h"
#include "cEntity.h"

class cWanderBehaviour : public iBehaviour {
public:
	cWanderBehaviour(cEntity* agent, float mMapExtents);
	virtual ~cWanderBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime);
	virtual bool isFinished();
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void findNewPosition(float extents);

private:
	cEntity* mAgent;
	float mMapExtents;
	glm::vec3 mTargetPosition;
};


#endif // !_cWanderBehaviour_HG_

