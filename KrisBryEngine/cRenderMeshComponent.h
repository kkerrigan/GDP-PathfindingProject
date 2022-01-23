#ifndef _cRenderMeshComponent_HG_
#define _cRenderMeshComponent_HG_

#include <gameMath.h>
#include <vector>
#include <string>

#include "cComponent.h"
#include "sTextureInfo.h"

#include <rapidjson/prettywriter.h>

class cRenderMeshComponent : public cComponent {
public:
	int shaderId;
	
	std::string meshName;
	std::string friendlyName;
	
	bool bIsWireFrame;	
	bool bIsVisible;
	bool bUseVertexColour;
	bool bDontLight;
	bool colorByHeight;

	glm::vec4 materialDiffuse;
	void setDiffuseColour(glm::vec3 newDiffuseColour);
	void setAlphaTransparency(float newAlpha);

	glm::vec4 materialSpecular;
	void setSpecularColour(glm::vec3 newSpecularColour);
	void setSpecularPower(float specPower);

	std::vector<cRenderMeshComponent*> vec_pChildObjectsToDraw;
	std::vector<sTextureInfo> textures;

	// Graphics Final Question 2
	bool bIsUpperWater;
	bool bIsLowerWater;

	cRenderMeshComponent();

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

private:
	std::string mType;
};

#endif
