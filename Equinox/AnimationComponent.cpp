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
	GetAnimationNames(animationNames);
	int current_type = 0;
	ImGui::Combo("Anim instance", &current_type, animationNames);
}

void AnimationComponent::CleanUp()
{
	free(animationNames);
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

void AnimationComponent::GetAnimationNames(char* animNames)
{
	AnimMap animMap = App->animator->Animations;
	char combo[256] = { 0 };
	size_t totalSize = 0;

	for (AnimMap::iterator it = animMap.begin(); it != animMap.end(); ++it)
	{
		std::string animName(it->first, '\0');
		size_t lenght = animName.size();
		memcpy(combo + totalSize, animName.c_str(), totalSize + lenght + 1);
		totalSize += lenght + 1;
	}
	animationNames = static_cast<char*>(malloc(totalSize + 1));
	animationNames[totalSize] = '\0';
	memcpy(animationNames, combo, totalSize);
}
