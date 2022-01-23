#ifndef _cCameraMoveToDistance_HG_
#define _cCameraMoveToDistance_HG_

#include "iCommand.h"
#include "cFlyCamera.h"

class cCameraMoveToDistance : public iCommand {
public:
	cCameraMoveToDistance(cFlyCamera* camera, glm::vec3 destination, float time, float easePercent = 0.0f, float easeOutPercent = 0.0f);
	void update(float deltaTime) override;
	bool isFinished() override;

private:
	cFlyCamera* camera;
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
