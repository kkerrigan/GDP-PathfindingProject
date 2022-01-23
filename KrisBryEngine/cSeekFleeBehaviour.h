/**
 * @file cSeekFleeBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will cause the agent entity to do one of 2 things depending on the target.
		1. Seek the target if the target is facing AWAY from agent.
		2. Flee the target if the target is facing TOWARDS the agent.
 */

#ifndef _cSeekFleeBehaviour_HG_
#define _cSeekFleeBehaviour_HG_

#include "iBehaviour.h"
#include "cEntity.h"

class cSeekFleeBehaviour : public iBehaviour {
public:
	cSeekFleeBehaviour(cEntity* agent, cEntity* target);
	virtual ~cSeekFleeBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;

private:
	cEntity* mAgent;
	cEntity* mTarget;
};

#endif // !_cSeekFleeBehaviour_HG_

