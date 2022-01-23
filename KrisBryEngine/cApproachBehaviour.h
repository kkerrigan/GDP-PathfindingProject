/**
 * @file cApproachBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will make an object move towards the player until a certain distance.
 */

#ifndef _cApproachBehaviour_HG_
#define _cApproachBehaviour_HG_

#include "iBehaviour.h"
#include "cEntity.h"

class cApproachBehaviour : public iBehaviour {
public:
	cApproachBehaviour(cEntity* pursuer, cEntity* target, float minPurseDist);
	virtual ~cApproachBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;

private:
	cEntity* mPursuer;
	cEntity* mTarget;
	float mMinPursueDistance;
};

#endif // !_cPursureBehaviour_HG_

