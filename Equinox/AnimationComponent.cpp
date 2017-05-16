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

void AnimationComponent::BeginPlay()
{
	meshComponent = static_cast<MeshComponent*>(Parent->GetComponentByName("MeshComponent"));
}


void AnimationComponent::Update(float DeltaTime)
{
	if(AnimInstanceID >= 0)
		TransformChild(Parent);

	if(meshComponent)
	{
		for(Mesh* mesh : meshComponent->Meshes)
		{
			UpdateVertexChild(Parent, mesh);
		}
	}
}

void AnimationComponent::DrawUI()
{
	ModuleAnimation* animator = App->animator;
	char* animationNames = animator->GetAnimationLabels();

	int current_type = animator->GetLabelByInstance(AnimInstanceID);
	ImGui::Combo("Anim instance", &current_type, animationNames);

	if(current_type == 0)
		AnimInstanceID = -1;

	if (!animator->isAnimInstanceOfAnim(
		AnimInstanceID,
		animator->GetAnimByName(animator->GetNameAnimByLabel(current_type))))
	{
		if(AnimInstanceID == -1)
			AnimInstanceID = animator->Play(animator->GetNameAnimByLabel(current_type).c_str());
		else
		{
			std::string animName = animator->GetNameAnimByLabel(current_type);
			animator->BlendTo(this->AnimInstanceID, animName.c_str(), 1500);
		}
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

void AnimationComponent::UpdateVertexChild(GameObject* parent, Mesh* mesh)
{
	for(GameObject* child : parent->GetChilds())
	{
		for(Bone* bone : mesh->Bones)
		{
			if (child->Name.compare(bone->Name) == 0)
			{
				//Use Skinning formula from Skinning.cpp to update each vertex
				//Update vertex array of mesh
			}
		}
		
	}

}
