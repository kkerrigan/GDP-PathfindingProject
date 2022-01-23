#ifndef _cFireCannons_HG_
#define _cFireCannons_HG_

#include <vector>

#include "Global.h"
#include "iCommand.h"
#include "cEntity.h"

class cFireCannons : public iCommand {
public:
	cFireCannons(cEntity* pAttacker, cEntity* pTarget);
	~cFireCannons();

	void update(float deltaTime) override;
	bool isFinished() override;

private:
	cEntity* pAttackingShip;
	cEntity* pTargetShip;
	std::vector<cEntity*> pCannonBalls;
	float reloadTime;
	bool bIsFinished;

	cEntity* chooseRandomBullet();
	float getRandInRange(float min, float max);
	unsigned int getRandInRange(unsigned int min, unsigned int max);
};

#endif // !_cFireCannons_HG_

