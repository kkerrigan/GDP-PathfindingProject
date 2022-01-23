/**
 * @file cCrowdFollowBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour causes a crowd (created using a cFormationComponent) to follow 
 * a predetermined path as a group.
 */

#ifndef _cCrowdFollowBehaviour_HG_
#define _cCrowdFollowBehaviour_HG_

#include "iCrowdBehaviour.h"
#include "cFormationBehaviour.h"
#include "PathingStructs.h"

class cCrowdFollowBehaviour : public iCrowdBehaviour {
public:
	cCrowdFollowBehaviour(cFormationBehaviour* formation);
	virtual ~cCrowdFollowBehaviour();

	virtual void registerEntity(cEntity* pEntity) override;
	virtual void unregisterEntity(cEntity* pEntity) override;
	virtual void update() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void addCheckpoint(sPathNode checkpoint, int index = -1);
	void setPath(sPath checkpoints);
	int getCurrentCheckpoint();
	sPath& getPath();

	void toggleDirection();
	void togglePathFollowing();
	bool mBIsFollowingPath;

private:
	cFormationBehaviour* mCrowdFormation;
	int mCurrentNode;
	int mDirection;
	bool mBIsBehaviourStarted;
	sPath mCheckpoints;
};


#endif // !_cCrowdFollowComponent_HG_

