/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cThreadSystem_HG_
#define _cThreadSystem_HG_

#include <vector>
#include <gameMath.h>

#include "iSystem.h"

class cThreadSystem : public iSystem {
public:
	static cThreadSystem* getInstance();
	cThreadSystem(cThreadSystem const&) = delete;
	void operator=(cThreadSystem const&) = delete;
	

	virtual void update(double deltaTime) override;
	virtual void registerEntity(cEntity* entity) override;
	virtual void unregisterEntity(cEntity* entity) override;

	void initBuffers(unsigned int size);

	// Called by the renderer (or main thread)
	void switchBuffers(void);

	unsigned int getCurrentReadBuffer(void);
	unsigned int getCurrentWriteBuffer(void);

private:
	cThreadSystem();
	virtual ~cThreadSystem();

	std::vector<cEntity*> mEntities;

	unsigned int m_CurrentReadBufferIndex;
	unsigned int m_CurrentWriteBufferIndex;

	static const unsigned int NUMBEROFBUFFERS = 3;
	std::vector<glm::vec3> vecPositions[NUMBEROFBUFFERS];
};


#endif // !_cThreadSystem_HG_

