#ifndef _cMeshRenderSystem_HG_
#define _cMeshRenderSystem_HG_

#include <vector>
#include <gameMath.h>

#include "Global.h"
#include "iSystem.h"
#include "cRenderMeshComponent.h"
#include "cSkinnedMeshComponent.h"

class cMeshRenderSystem : public iSystem {
public:

	GLuint skyBoxTextureUnitID;

	// Singleton setup
	static cMeshRenderSystem* getInstance();
	cMeshRenderSystem(cMeshRenderSystem const&) = delete;
	void operator=(cMeshRenderSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);
	void bindTextures(cEntity* pCurrentEntity);
	void drawSkyBox();
	void drawObject(cEntity* pCurrentMesh, glm::mat4x4 &matModel);
	void drawObjectInstances(cEntity* pCurrentMesh);
	void drawScene_Simple(std::vector<cEntity*> pCurrentMesh, unsigned int passNumber);

private:
	std::vector<cEntity*> entitiesToUpdate;

	cMeshRenderSystem();
	~cMeshRenderSystem();
};

#endif
