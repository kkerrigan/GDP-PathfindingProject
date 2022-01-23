/**
 * @file cFlockBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour will cause the entities to form a multi-agent (local) flock
 * that adheres to Craig Reynolds Flocking rules:
 *		- Cohesion
 *		- Alignment
 *		- Separation
 */

#ifndef _cFlockBehaviour_HG_
#define _cFlockBehaviour_HG_

#include "iBehaviour.h"
#include "cTransformComponent.h"

class cFlockBehaviour : public iBehaviour {
public:

	// Define a struct to hold the information for each Boid
	struct sBoid {

		cEntity* entity;
		cTransformComponent* transform;
	};

	cFlockBehaviour();
	virtual ~cFlockBehaviour();

	// The weights for the rules mentioned in the description
	float cohesionWeight;
	float alignmentWeight;
	float separationWeight;
	glm::vec3 flockDirection;
	glm::vec3 flockCenter;

	virtual void update(cEntity* pEntity, float deltaTime) override;
	virtual bool isFinished() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void addBoid(sBoid boid);
	std::vector<sBoid>& getFlockMembers();

private:
	glm::vec3 mCohesion;
	glm::vec3 mAlignment;
	glm::vec3 mSeparation;
	bool mBIsBehaviourStarted;
	std::vector<sBoid> mBoidsInFlock;

	void mCalculateBoidCohesion(cEntity* boid, glm::vec3& cohesion);
	void mCalculateBoidSeparation(cEntity* boid, glm::vec3& separation);
	void mCalculateBoidAlignment(cEntity* boid, glm::vec3& alignment);
};

#endif // !_cFlockBehaviour_HG_

