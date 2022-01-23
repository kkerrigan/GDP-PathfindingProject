#ifndef _cAnimationState_HG_
#define _cAnimationState_HG_

#include <string>
#include <queue>

class cAnimationState {
public:
	cAnimationState() {}

	struct sStateDetails {
		sStateDetails() : 
			currentTime(0.0f), 
			totalTime(0.0f),
			frameStepTime(0.0f),
			bDidWeReset(false){};
		
		std::string name;
		float currentTime;		// Time (frame) in current animation
		float totalTime;		// Total time animation goes
		float frameStepTime;	// Number of seconds to 'move' the animation
		float elapsedTime;
		bool bDidWeReset;
		
		// Returns true if time had to be reset
		// (for checking to see if the animation has finished or not)
		// TODO: Deal with running the animation backwards, perhaps?? 
		bool incrementTime(float deltaTime, bool bResetToZero = true);
	};
	
	std::queue<sStateDetails> animationQueue;
	sStateDetails defaultAnimation;

	sStateDetails& getRunningAnimation();
};

#endif 

