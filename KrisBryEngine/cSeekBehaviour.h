/**
 * @file cSeekBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour causes the seeker entity to chase (Seek) the target entity.
 */

#ifndef _cSeekBehaviour_HG_
#define _cSeekBhaviour_HG_

#include "iBehaviour.h"
#include "cEntity.h"

class cSeekBehaviour : public iBehaviour {
public:
	cSeekBehaviour(cEntity* seeker, cEntity* target);
	virtual ~cSeekBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

private:
	cEntity* mSeeker;
	cEntity* mTarget;
};

#endif // !_cSeekBehaviour_HG_

