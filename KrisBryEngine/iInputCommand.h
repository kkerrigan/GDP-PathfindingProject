/*
* All the commands for the mediator (This will be cleaned up and moved into thier own files
* to limit the impact on build time (this is here to demo easily)
*/

#ifndef _iCommand_HG_
#define _iCommand_HG_

#include <iostream>

#include "cFlyCamera.h"
#include "cEntity.h"
#include "sLight.h"
#include "cSoundManager.h"
#include "c3dSoundComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"

class iInputCommand {
public:
	virtual void execute(void* thingIAmDoingWorkOn) = 0;
	virtual ~iInputCommand() {}

};

class cMoveCameraForwardCommand : public iInputCommand{
	float distance;
public:
	cMoveCameraForwardCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveForward_Z(distance);
	}
};

class cMoveCameraLeftRightCommand : public iInputCommand {
	float distance;
public:
	cMoveCameraLeftRightCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveLeftRight_X(distance);
	}
};

class cMoveCameraUpDownCommand : public iInputCommand {
	float distance;
public:
	cMoveCameraUpDownCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveUpDown_Y(distance);
	}
};

class cRollCameraCommand : public iInputCommand {
	float distance;
public:
	cRollCameraCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIamDoingWorkOn) {
		((cFlyCamera*)thingIamDoingWorkOn)->Roll_CW_CCW(distance);
	}
};

class cMoveLightForwardCommand : public iInputCommand {
	float distance;
public:
	cMoveLightForwardCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.z += distance;
	}
};

class cMoveLightLeftRightCommand : public iInputCommand {
	float distance;
public:
	cMoveLightLeftRightCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.x += distance;
	}
};

class cMoveLightUpDownCommand : public iInputCommand {
	float distance;
public:
	cMoveLightUpDownCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.y += distance;
	}
};

class cLightChangeConstAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeConstAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.x *= multiplier;
	}
};

class cLightChangeLinAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeLinAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.y *= multiplier;
	}
};

class cLightChangeQuadAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeQuadAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.z *= multiplier;
	}
};

class cMoveEntiyForwardCommand : public iInputCommand {
	float distance;
public:
	cMoveEntiyForwardCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		glm::vec3 pos = ((cTransformComponent*)thingIAmDoingWorkOn)->getPosition();
		pos.z += distance;
		((cTransformComponent*)thingIAmDoingWorkOn)->setPosition(pos);
	}
};

class cMoveEntiyLeftRightCommand : public iInputCommand {
	float distance;
public:
	cMoveEntiyLeftRightCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		glm::vec3 pos = ((cTransformComponent*)thingIAmDoingWorkOn)->getPosition();
		pos.x += distance;
		((cTransformComponent*)thingIAmDoingWorkOn)->setPosition(pos);
	}
};

class cMoveEntiyUpDownCommand : public iInputCommand {
	float distance;
public:
	cMoveEntiyUpDownCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		glm::vec3 pos = ((cTransformComponent*)thingIAmDoingWorkOn)->getPosition();
		 pos.y += distance;
		 ((cTransformComponent*)thingIAmDoingWorkOn)->setPosition(pos);
	}
};

class cRotateEntiyZCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyZCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(0.0f, 0.0f, delta), true);
	}
};

class cRotateEntiyXCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyXCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(delta, 0.0f, 0.0f), true);
	}
};

class cRotateEntiyYCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyYCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(0.0f, delta, 0.0f), true);
	}
};

class cChangeEntitySpecularPower : public iInputCommand {
	float multiplyer;
public:
	cChangeEntitySpecularPower(float multiplyer_) : multiplyer(multiplyer_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cRenderMeshComponent*)thingIAmDoingWorkOn)->materialSpecular.a *= multiplyer;

			if (((cRenderMeshComponent*)thingIAmDoingWorkOn)->materialSpecular.a < 1.0) {
				((cRenderMeshComponent*)thingIAmDoingWorkOn)->setSpecularPower(1.0f);
			}
	}
};

#endif
