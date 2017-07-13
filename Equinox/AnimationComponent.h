#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "BaseComponent.h"
#include "ModuleAnimation.h"

class AnimationComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION;
public:
	AnimationComponent();
	~AnimationComponent();

	void Update(float DeltaTime) override;
	void DrawUI() override;

	void TransformChild(GameObject* parent) const;

public:
	AnimInstanceID AnimInstanceID = -1;

};

#endif
