#include "AnimationComponent.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "IMGUI/imgui.h"

AnimationComponent::AnimationComponent()
{
	Name = "Animation";
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::Update()
{
	if(AnimInstanceID >= 0)
		TransformChild(Parent);
}

void AnimationComponent::DrawUI()
{
	char* animationNames = App->animator->GetAnimationLabels();

	int current_type = App->animator->GetLabelByInstance(AnimInstanceID);
	ImGui::Combo("Anim instance", &current_type, animationNames);
	if (!App->animator->isAnimInstanceOfAnim(
		AnimInstanceID,
		App->animator->GetAnimByLabel(current_type)))
	{
		LOG("IT HAS CHANGED");
	}
}

void AnimationComponent::TransformChild(GameObject* parent) const
{
	for (GameObject* child : parent->GetChilds())
	{
		TransformComponent* childTransform = child->GetTransform();
		App->animator->GetTransform(AnimInstanceID, child->Name.c_str(),
			childTransform->Position, childTransform->Rotation);
		
		TransformChild(child);
	}
}
