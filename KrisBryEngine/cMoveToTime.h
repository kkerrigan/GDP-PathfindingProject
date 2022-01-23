#ifndef _cMoveToTime_HG_
#define _cMoveToTime_HG_

#include <gameMath.h>

#include "iCommand.h"
#include "cEntity.h"

class cMoveToTime : public iCommand {
public:
	cMoveToTime(cEntity* entity, glm::vec3 destination, float time, float easeInTime = 0.0f, float easeOutTime = 0.0f);
	void update(float deltaTime) override;
	bool isFinished() override;

private:
	cEntity* entity;
	glm::vec3 destination;
	float time;
	float easeInTime;
	float easeOutTime;
	float commandRunTime;
	float approxVelocityRequired;
	bool finished;

	float findMaxVelocity();
};

#endif
