/**
 * @file cFollowBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This component will make an individual entity follow a pre-defined path.
 */

#ifndef _cFollowBehaviour_HG_
#define _cFollowBehaviour_HG_

#include <gameMath.h>

#include "iBehaviour.h"
#include "PathingStructs.h"

class cFollowBehaviour : public iBehaviour {
public:
	cFollowBehaviour();
	virtual ~cFollowBehaviour();

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;

	void addCheckpoint(sPathNode checkpoint, int index = -1);
	void setPath(sPath path);
	sPath& getPath();

private:
	int mCurrentNode;
	int mDirection;
	sPath mPath;
};

#endif // !_cFollowBehaviour_HG_

