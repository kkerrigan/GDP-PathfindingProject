#ifndef _cMoveToDistance_HG_
#define _cMoveToDistance_HG_

#include <gameMath.h>
#include "iCommand.h"


class cEntity;

class cMoveToDistance : public iCommand {
public:
	cMoveToDistance(cEntity* entity, glm::vec3 destination, float time, float easePercent = 0.0f, float easeOutPercent = 0.0f);
	void update(float deltaTime) override;
	bool isFinished() override;

private:
	cEntity* entity;
	glm::vec3 destination;
	float time;
	float easeInPercent;
	float easeOutPercent;
	float commandRunTime;
	bool finished;

	float distanceToTravel;
	float distanceTraveled;

	float approxVelocityRequired;
};

#endif
