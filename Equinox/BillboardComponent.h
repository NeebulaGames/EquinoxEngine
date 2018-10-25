#pragma once
#include "BaseComponent.h"
#include "CameraComponent.h"

class Billboard;

class BillboardComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION
public:
	BillboardComponent();
	~BillboardComponent();

	void Update(float dt) override;
	void EditorUpdate(float dt) override;
	void DrawUI() override;

	void SetTexture(unsigned textureId) const;

private:
	Billboard* _billboard;

	void ComputeQuad(const CameraComponent& camera, float3& up, float3& right) const;
};

