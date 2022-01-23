/**
 * @file cBehaviourComponent.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class holds all the behaviours an individual cEntity has.
 */

#ifndef _cBehaviourComponent_HG_
#define _cBehaviourComponent_HG_

#include <vector>
#include <string>

#include "cComponent.h"
#include "iBehaviour.h"

class cBehaviourComponent : public cComponent {
public:
	cBehaviourComponent() : cComponent() {
		
		this->mType = "Behaviour";
	}

	template<class T> T* addBehaviour();
	template<class T> T* getBehaviour();
	template<class T> bool hasBehaviour();

	std::vector<iBehaviour*>& getBehaviours();

private:
	std::vector<iBehaviour*> mBehaviours;
	std::string mType;
};

/**
 * addBehaviour
 * Adds a behaviour of the provided type to the entity's behaviour component
 * @return - The behaviour that was created
 */
template <class T> T* cBehaviourComponent::addBehaviour() {

	T * behaviour = new T();
	assert(behaviour);

	if (dynamic_cast<iBehaviour*>(behaviour) == 0) return 0;

	this->mBehaviours.push_back(behaviour);

	return behaviour;

}

/**
 * hasBehaviour
 * Checks the container of behaviours if there is a behaviours of the
 * provided type in it.
 * @return - TRUE if found, FALSE otherwise
 */
template <class T> bool cBehaviourComponent::hasBehaviour() {

	for (size_t i = 0; i < this->mBehaviours.size(); ++i) {

		if (dynamic_cast<T*>(this->mBehaviours[i]) != 0) return true;
	}

	return false;

}

/**
 * GetBehaviour
 * Checks if the cBehaviourComponent of the entity contains a behaviour of the provided type
 * @return - The behaviour if found, 0 otherwise
 */
template <class T> T* cBehaviourComponent::getBehaviour() {

	for (size_t i = 0; i < this->mBehaviours.size(); ++i) {

		if (dynamic_cast<T*>(this->mBehaviours[i]) != 0) {

			return dynamic_cast<T*>(this->mBehaviours[i]);
		}
	}

	return 0;

}

#endif // !_cBehaviourComponent_HG_
