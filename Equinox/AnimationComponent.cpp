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
	ModuleAnimation* animator = App->animator;
	char* animationNames = animator->GetAnimationLabels();

	int current_type = animator->GetLabelByInstance(AnimInstanceID);
	ImGui::Combo("Anim instance", &current_type, animationNames);
	if (!animator->isAnimInstanceOfAnim(
		AnimInstanceID,
		animator->GetAnimByName(animator->GetNameAnimByLabel(current_type))))
	{
		const char* name = animator->GetNameAnimByLabel(current_type).c_str();
		LOG("IT HAS CHANGED TO %s", name);
		int id = this->AnimInstanceID;
		this->AnimInstanceID = animator->Play(name);
		animator->Stop(id);
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
