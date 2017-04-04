#include "AnimationComponent.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "IMGUI/imgui.h"
#include "GameObject.h"

AnimationComponent::AnimationComponent()
{
	Name = "Animation";
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::Update(float DeltaTime)
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
		std::string name = animator->GetNameAnimByLabel(current_type);
		LOG("IT HAS CHANGED TO %s", name.c_str());
		int id = this->AnimInstanceID;
		this->AnimInstanceID = animator->Play(name.c_str());
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
