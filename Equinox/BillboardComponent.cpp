#include "BillboardComponent.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "Billboard.h"

BillboardComponent::BillboardComponent()
{
	Name = "Billboard";
	_billboard = new Billboard;
}


BillboardComponent::~BillboardComponent()
{
}

void BillboardComponent::Update(float dt)
{
	float3 up, right;
	ComputeQuad(*App->editorCamera->GetCamera(), up, right);
	_billboard->Draw(Parent->GetTransform()->GetPosition(), up, right);
}

void BillboardComponent::EditorUpdate(float dt)
{
	Update(dt);
}

void BillboardComponent::DrawUI()
{
}

void BillboardComponent::SetTexture(unsigned textureId) const
{
	_billboard->SetTexture(textureId);
}

void BillboardComponent::ComputeQuad(const CameraComponent& camera, float3& up, float3& right) const
{
	TransformComponent* transform = Parent->GetTransform();

	if (transform)
	{
		float3 position = transform->GetPosition();
		up = float3::unitY;
		right = (position - camera.Position()).Normalized().Cross(up);
		right.Normalize();
	}
}
