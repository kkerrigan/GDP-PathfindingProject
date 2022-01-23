/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Implementation of the cBehaviour class.
 */

#include "cBehaviourComponent.h"

 std::vector<iBehaviour*>& cBehaviourComponent::getBehaviours() {
	
	return this->mBehaviours;
}
