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

	void Update() override;
	void DrawUI() override;
	void CleanUp() override;

	void TransformChild(GameObject* parent) const;

public:
	AnimInstanceID AnimInstanceID;

private:
	void GetAnimationNames(char* animNames);
	char* animationNames;
};

#endif
