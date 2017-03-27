#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "BaseComponent.h"
#include "ModuleAnimation.h"

class AnimationComponent :
	public BaseComponent
{
public:
	AnimationComponent();
	~AnimationComponent();

	AnimInstanceID AnimInstanceID;
};

#endif
