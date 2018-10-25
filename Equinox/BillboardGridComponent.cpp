#include "BillboardGridComponent.h"
#include "Billboard.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Engine.h"
#include "ModuleEditorCamera.h"


BillboardGridComponent::BillboardGridComponent()
{
}


BillboardGridComponent::~BillboardGridComponent()
{
}

void BillboardGridComponent::Update(float dt)
{
	float3 objPosition = Parent->GetTransform()->GetPosition();
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			Billboard billboard;
			billboard.SetTexture(_textureId);
			float3 up, right;
			int w, h;
			billboard.GetSize(w, h);
			
			float3 position = objPosition;
			position.x = position.x - n / 2 * float(w / 2.f) + float(w / 2.f) * i;
			position.z = position.z - m / 2 + j / 2.f;

			ComputeQuad(position, App->editorCamera->GetCamera()->Position(), up, right);
			billboard.Draw(position, up, right);
		}
	}
}

void BillboardGridComponent::EditorUpdate(float dt)
{
	Update(dt);
}

void BillboardGridComponent::DrawUI()
{
}

void BillboardGridComponent::SetTexture(unsigned textureId)
{
	_textureId = textureId;
}

void BillboardGridComponent::ComputeQuad(const float3& position, const float3& cameraPosition, float3& up, float3& right) const
{
	TransformComponent* transform = Parent->GetTransform();

	if (transform)
	{
		up = float3::unitY;
		right = (position - cameraPosition).Normalized().Cross(up);
		right.Normalize();
	}
}
