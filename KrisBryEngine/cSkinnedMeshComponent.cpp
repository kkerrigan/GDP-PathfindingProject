#include "cSkinnedMeshComponent.h"

#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"
#include "cShaderManager.h"
#include "cMesh.h"

// c'tor
cSkinnedMeshComponent::cSkinnedMeshComponent() : cComponent() {

	this->pSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	this->pAniState = nullptr;
	this->mType = "Skin Mesh";

	return;
}