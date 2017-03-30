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
	TransformChild(Parent);
}

void AnimationComponent::DrawUI()
{
	AnimMap animMap = App->animator->Animations;
	char combo[256] = { 0 };
	for (AnimMap::iterator it = animMap.begin(); it != animMap.end(); ++it)
	{
		strcat_s(combo, std::string(it->first).c_str());
	}
		
		
	int current_type = 0;
	ImGui::Combo("Anim instance", &current_type, combo);
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
