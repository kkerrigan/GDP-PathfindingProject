#include "cRenderMeshComponent.h"

// c'tor
cRenderMeshComponent::cRenderMeshComponent() : cComponent(), colorByHeight(false) {

	this->materialDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->bIsUpperWater = false;
	this->bIsLowerWater = false;
	this->mType = "Render";

	return;

}

void cRenderMeshComponent::setDiffuseColour(glm::vec3 newDiffuse) {

	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}


void cRenderMeshComponent::setAlphaTransparency(float newAlpha) {

	this->materialDiffuse.a = newAlpha;
	return;
}


void cRenderMeshComponent::setSpecularColour(glm::vec3 colourRGB) {

	this->materialSpecular = glm::vec4(colourRGB, this->materialSpecular.a);
	return;
}


void cRenderMeshComponent::setSpecularPower(float specPower) {

	this->materialSpecular.a = specPower;
	return;
}

void cRenderMeshComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	//Start Component
	writer.StartObject();

	writer.String("componentType");
	writer.String(this->mType.c_str());

	writer.String("meshName");
	writer.String(this->meshName.c_str());

	writer.String("isWireframe");
	writer.Bool(this->bIsWireFrame);

	writer.String("isVisible");
	writer.Bool(this->bIsVisible);

	// Colour
	writer.String(("color"));
	writer.StartObject();

	writer.String("r");
	writer.Double(this->materialDiffuse.r);
	writer.String("g");
	writer.Double(this->materialDiffuse.g);
	writer.String("b");
	writer.Double(this->materialDiffuse.b);
	writer.String("a");
	writer.Double(this->materialDiffuse.a);
	writer.EndObject();

	writer.String("textures");
	writer.StartArray();

	for (size_t i = 0; i < this->textures.size(); ++i) {
		writer.StartObject();

		writer.String("name");
		writer.String(this->textures[i].name.c_str());

		writer.String("strength");
		writer.Double(this->textures[i].strength);

		writer.EndObject();
	}

	writer.EndArray();

	// Specular
	writer.String("specular");
	writer.StartObject();
	writer.String("r");
	writer.Double(this->materialSpecular.r);
	writer.String("g");
	writer.Double(this->materialSpecular.g);
	writer.String("b");
	writer.Double(this->materialSpecular.b);
	writer.String("shinyness");
	writer.Double(this->materialSpecular.a);
	writer.EndObject();

	writer.String("useVertexColor");
	writer.Bool(this->bUseVertexColour);

	writer.String("friendlyName");
	writer.String(this->friendlyName.c_str());

	writer.String("dontLight");
	writer.Bool(this->bDontLight);

	writer.EndObject();  // End Component
}
