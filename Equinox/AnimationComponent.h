#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "BaseComponent.h"
#include "ModuleAnimation.h"
#include "MeshComponent.h"

class AnimationComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION;
public:
	AnimationComponent();
	~AnimationComponent();

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void DrawUI() override;

	void TransformChild(GameObject* parent) const;

public:
	AnimInstanceID AnimInstanceID = -1;
	MeshComponent* meshComponent = nullptr;
};

#endif
