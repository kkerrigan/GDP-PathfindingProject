/**
 * @file cIdleBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will cause the entity to idle at its position for the provided amount of time.
 */

#ifndef _cIdleBehaviour_HG_
#define _cIdleBehaviour_HG_

#include "iBehaviour.h"
#include "cEntity.h"

class cIdleBehaviour : public iBehaviour {
public:
	cIdleBehaviour(float timeToWait);
	virtual ~cIdleBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void refreshIdle();

private:
	float mTimeToWait;
	float mTotalTime;
};

#endif // !_cIdleBehaviour_HG_

