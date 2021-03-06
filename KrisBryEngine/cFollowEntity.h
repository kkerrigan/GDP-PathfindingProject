#ifndef _cFollowEntity_HG_
#define _cFollowEntity_HG_

#include <gameMath.h>
#include "iCommand.h"
#include "cEntity.h"

class cFollowEntity : public iCommand {
public:
	cFollowEntity(cEntity* entity, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
	              float maxSpeedDistance, float maxSpeed, glm::vec3 offset, float easeInRange, float easeOutRange);

	void update(float deltaTime) override;
	bool isFinished() override;

private:
	cEntity* entity;
	cEntity* entityToFollow;
	glm::vec3 followDistance;
	glm::vec3 offset;
	float minDistance;
	float maxSpeed;
	float maxSpeedDistance;

	//note: the easing is handled via the max speed distance
	float easeInRange;
	float easeOutRange;

	bool orientToTarget;
	bool finished;
};

#endif
