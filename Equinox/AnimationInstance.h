#ifndef __ANIMATIONINSTANCE_H__
#define __ANIMATIONINSTANCE_H__

#include "ModuleAnimation.h"

class AnimationInstance
{
public:
	Animation* animation = nullptr;
	float localTime = 0.0f;
	bool loop = true;
	

public:
	AnimationInstance()
	{}

	AnimationInstance(const AnimationInstance& anim) : animation(anim.animation), localTime(anim.localTime), loop(anim.loop)
	{}


	void Reset()
	{
		localTime = 0.0f;
	}
};

#endif // __ANIMATIONINSTANCE_H__
