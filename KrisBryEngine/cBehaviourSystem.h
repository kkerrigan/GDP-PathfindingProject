/**
 * @file cBehaviourSystem.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Processes all the different behaviours on the entities.
 */

#ifndef _cBehaviourSystem_HG_
#define _cBehaviourSystem_HG_

#include <vector>

#include "iSystem.h"
#include "iCrowdBehaviour.h"

class cBehaviourSystem : public iSystem {
public:

	static cBehaviourSystem* getInstance();
	cBehaviourSystem(cBehaviourSystem const&) = delete;
	void operator=(cBehaviourSystem const&) = delete;

	virtual void update(double deltaTime) override;
	virtual void registerEntity(cEntity* entity) override;
	virtual void unregisterEntity(cEntity* entity) override;

	std::vector<cEntity*> getEntities() const;
	std::vector<iCrowdBehaviour*> getCrowdBehaviours() const;

	// Crowd behaviour (formation) functions
	void registerCrowdBehaviour(iCrowdBehaviour* behaviour);
	void unregisterCrowdBehaviour(iCrowdBehaviour* behaviour);

private:
	cBehaviourSystem() {}
	virtual ~cBehaviourSystem() {}

	std::vector<cEntity*> mEntities;
	std::vector<iCrowdBehaviour*> mCrowdBehaviours;
};

#endif // !_cBehaviourSystem_HG_

