/**
 * @file cFleeBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour causes one entity to Flee (run away from) another entity.
 */

#ifndef _cFleeBehaviour_HG_
#define _cFleeBehaviour_HG_

#include "iBehaviour.h"
#include "cEntity.h"

class cFleeBehaviour : public iBehaviour {
public:
	cFleeBehaviour(cEntity* fleeEntity, cEntity* targetEntity);
	virtual ~cFleeBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

private:
	cEntity* mFleeEntity;
	cEntity* mTargetEntity;
};

#endif // !_cFleeBehaviour_HG_

