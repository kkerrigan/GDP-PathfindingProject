/**
 * @file cInstancedMeshComponent.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cInstancedMeshComponent_HG_
#define _cInstancedMeshComponent_HG_

#include <gameMath.h>
#include <vector>
#include <map>
#include <string>

#include "cComponent.h"
#include "PathingStructs.h"

class cInstancedMeshComponent : public cComponent {
public:
	
	struct sMesh {
		int ID;
		glm::vec3 position;
		glm::vec4 color;
	};

	struct sDrawInfo {
		size_t numInstances;
		std::vector<sMesh*> instances;
	};

	std::vector<sMesh> instances;

	cInstancedMeshComponent();
	virtual ~cInstancedMeshComponent();

	std::map<std::string, sDrawInfo> generateDrawInfo();
	void updateInstances(std::vector<sPathFindingNode*> nodes, bool renderTraversable = false);
};

#endif // !_cInstancedMeshComponent_HG_

