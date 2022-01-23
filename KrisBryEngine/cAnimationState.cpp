#include "cAnimationState.h"

cAnimationState::sStateDetails& cAnimationState::getRunningAnimation() {

	if (this->animationQueue.empty()) {

		return this->defaultAnimation;
	}
	else {

		cAnimationState::sStateDetails& currAnimation = this->animationQueue.front();

		if (currAnimation.bDidWeReset) {

			// If this animation finished in the frame, remove from queue and try again.
			this->animationQueue.pop();

			return this->getRunningAnimation();
		}

		return currAnimation;
	}
}


// Returns true if time had to be reset
// (for checking to see if the animation has finished or not)
bool cAnimationState::sStateDetails::incrementTime(float deltaTime, bool bResetToZero /*=true*/) {

	this->elapsedTime += deltaTime;
	this->bDidWeReset = false;
	this->currentTime += this->frameStepTime;
	
	if (this->frameStepTime <= 0.0f) {

		this->frameStepTime = 0.01f;
	}
	
	if (this->currentTime >= this->totalTime) {
		
		bDidWeReset = true;
		this->currentTime = 0.0f;
	}

	return bDidWeReset;
}
