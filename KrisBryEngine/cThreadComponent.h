/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

// Gems Midterm Q3

#ifndef _cThreadComponent_HG_
#define _cThreadComponent_HG_

#include <gameMath.h>
#include <Windows.h>
#include <process.h>

#include "Ply/CHRTimer.h"
#include "cComponent.h"
#include "cEntity.h"

class cRandThreaded;

class cThreadComponent : public cComponent {
public:
	cThreadComponent();
	virtual ~cThreadComponent() {}

	void createThreadAndStartUpdating(cEntity* pEntity);
	void update(void);

	// When set to false, the thread will exit, stopping any updates
	bool bIsAlive;

	void resetTimerAndStart(void);

	// Will pick a random direction and move for x seconds
	void pickNewDirection(void);

private:
	float m_TimeToMove;
	glm::vec3 m_Velocity;

	CHRTimer* m_pLocalTimer;
	cRandThreaded* m_pRandThread;
	cEntity* pEntity;
	glm::vec3 mTarget;

	int m_CheckDirection(glm::vec3 position, int direction, int count = 0);
	glm::vec3 m_IncrementPosition(glm::vec3& currentPosition, int direction);
};


#endif // !_cThreadComponent_HG_

