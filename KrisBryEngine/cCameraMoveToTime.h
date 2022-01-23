#ifndef _cCameraMoveToTime_HG_
#define _cCameraMoveToTime_HG_

#include <gameMath.h>

#include "iCommand.h"
#include "cFlyCamera.h"

class cCameraMoveToTime : public iCommand {
public:
	cFlyCamera* camera;
	glm::vec3 destination;
	float time;
	float easeInTime;
	float easeOutTime;
	float commandRunTime;
	bool finished;

	float approxVelocityRequired;

	cCameraMoveToTime(cFlyCamera* entity, glm::vec3 destination, float time, float easeInTime = 0.0f, float easeOutTime = 0.0f);

	float findMaxVelocity();

	void update(float deltaTime) override;
	bool isFinished() override;
};

#endif
