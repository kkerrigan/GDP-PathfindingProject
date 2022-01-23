#ifndef _cEntity_HG_
#define _cEntity_HG_

#include <rapidjson/prettywriter.h>

#include <assert.h>
#include <vector>
#include <string>

#include "cComponent.h"

class cEntity {	
public:
	cEntity(int type);
	virtual ~cEntity();

	template<class T> T* addComponent();
	template<class T> T* getComponent();
	template<class T> bool hasComponent();
	void removeComponent(cComponent* comp);

	int UNIQUE_ID;
	const int entityType;
	std::vector<cComponent*> components;

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

private:
	static int nextUniqueId;
};

typedef std::vector<cEntity*>::iterator entityIter;

/**
 * addComponent
 * Adds a component of the provided type to the entity
 * @return - The component that was created
 */
template <class T> T* cEntity::addComponent() {

	T* component = new T();
	assert(component);

	if (dynamic_cast<cComponent*>(component) == 0) return 0;

	this->components.push_back(component);

	return component;
}

/**
 * hasComponent
 * Checks the container of components if there is a component of the
 * provided type in it.
 * @return - TRUE if found, FALSE otherwise
 */
template <class T> bool cEntity::hasComponent() {

	for (size_t i = 0; i < this->components.size(); ++i) {

		if (dynamic_cast<T*>(this->components[i]) != 0) return true;
	}

	return false;
}

/**
 * GetComponent
 * Checks if the entity contains a component of the provided type
 * @return - The component if found, 0 otherwise
 */
template <class T> T* cEntity::getComponent() {

	for (size_t i = 0; i < this->components.size(); ++i) {

		if (dynamic_cast<T*>(this->components[i]) != 0) {

			return dynamic_cast<T*>(this->components[i]);
		}
	}

	return 0;
}

#endif
