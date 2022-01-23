#ifndef _cCameraOrientToEntity_HG_
#define _cCameraOrientToEntity_HG_

#include "iCommand.h"
#include "Global.h"
#include "cEntity.h"
#include "cFlyCamera.h"

class cCameraOrientToEntity : public iCommand {
public:
	cCameraOrientToEntity(cFlyCamera* camera, cEntity* entityTarget, float duration, float easeIn, float easeOut);

	void update(float deltaTime) override;
	bool isFinished() override;
	
private:
	cFlyCamera* camera;
	cEntity* entityTarget;

	bool firstUpdateDone;
	double initialTime;
	double elapsedTime;
	float duration;

	glm::vec3 initialPosition;
	glm::vec3 theUpVector;
	glm::vec3 lookAtPosition;
	glm::mat4 initialOrientation;
	glm::mat4 finalOrientation;

	float easeInDistance;
	float easeOutDistance;

	bool finished;
};

#endif
