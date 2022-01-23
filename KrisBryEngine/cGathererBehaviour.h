/**
 * @file cGathererBehaviour.hpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will allow the entity to gather the resources.
 * The Gatherer can be in the following states:
 *		- Idle at Base
 *		- Idle at Resource
 *		- Finding Resource
 *		- Moving to Resource
 *		- Finding Home Base
 *		- Travel to Home Base
 */

#ifndef _cGathererBehaviour_HG_
#define _cGathererBehaviour_HG_

#include "iBehaviour.h"

class cScene;
class cGathererBehaviour : public iBehaviour {
public:
	enum eGatherStates {
		BASE_IDLE,
		RESOURCE_IDLE,
		FIND_RESOURCE,
		MOVE_TO_RESOURCE,
		FIND_HOME_BASE,
		TRAVEL_TO_HOME
	};

	cGathererBehaviour();
	virtual ~cGathererBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void setHomeBaseWaitTime(float waitTime);
	void setResourceWaitTime(float waitTime);
	void setActiveScene(cScene* scene);
	void setActiveBehaviour(iBehaviour* behaviour);

private:
	float mElapsedStateTime;
	float mHomeBaseWaitTime;
	float mResourceWaitTime;

	cScene* mScene;
	iBehaviour* mActiveBehaviour;
	eGatherStates mCurrentState;
};

#endif // !_cGathererBehaviour_HG_

