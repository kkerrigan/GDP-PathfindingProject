/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cFBOSystem_HG_
#define _cFBOSystem_HG_

#include <vector>

#include "Global.h"
#include "iSystem.h"

class cFBOSystem : public iSystem {
public:
	// Singleton setup
	static cFBOSystem* getInstance();
	cFBOSystem(cFBOSystem const&) = delete;
	void operator=(cFBOSystem const&) = delete;


	virtual void update(double deltaTime) override;
	virtual void registerEntity(cEntity* entity) override;
	virtual void unregisterEntity(cEntity* entity) override;

	void clearFbo();

private:
	cFBOSystem();
	virtual ~cFBOSystem();

	std::vector<cEntity*> entitiesToUpdate;
};



#endif // !_cFBOSystem_HG_

