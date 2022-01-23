/**
 * @file cFormationBehaviour.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This behaviour causes the attached entities to move in a specific pattern together.
 */

#ifndef _cFormationBehaviour_HG_
#define _cFormationBehaviour_HG_

#include <gameMath.h>

#include "iCrowdBehaviour.h"

class cFormationBehaviour : public iCrowdBehaviour {
public:
	enum eFormationTypes {
		LINE,
		SQUARE,
		CIRCLE,
		VSHAPE,
		TWO_ROWS,
		NONE
	};

	cFormationBehaviour();
	virtual ~cFormationBehaviour();

	virtual void registerEntity(cEntity* pEntity) override;
	virtual void unregisterEntity(cEntity* pEntity) override;
	virtual void update() override;
	virtual void startBehaviour() override;
	virtual void stopBehaviour() override;
	virtual bool isStarted() override;

	void setFormation(eFormationTypes formation);
	
	glm::vec3 getCrowdNucleus();
	void setCrowdNucleus(glm::vec3 newNucleus);

	void setCrowdDirection(glm::vec3 newDirection);

private:
	eFormationTypes mCurrentFormation;
	glm::vec3 mCrowdNucleus;
	glm::vec3 mCrowdDirection;
	bool mBIsBehaviourStarted;
	std::vector<cEntity*> mFormationEntities;

	std::vector<glm::vec3> mGenerateOffsets(eFormationTypes formation);
	std::vector<glm::vec3> mGenerateLineFormation();
	std::vector<glm::vec3> mGenerateSquareFormation();
	std::vector<glm::vec3> mGenerateCircleFormation();
	std::vector<glm::vec3> mGenerateVShapeFormation();
	std::vector<glm::vec3> mGenerateTwoRowsFormation();
};

#endif // !_cFormationComponent_HG_

