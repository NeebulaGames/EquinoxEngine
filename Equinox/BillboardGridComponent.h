#pragma once
#include "BaseComponent.h"

class CameraComponent;

class BillboardGridComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION
public:
	BillboardGridComponent();
	~BillboardGridComponent();

	void Update(float dt) override;
	void EditorUpdate(float dt) override;
	void DrawUI() override;

	void SetTexture(unsigned textureId);

	int n, m;

private:
	void ComputeQuad(const float3& position, const float3& cameraPosition, float3& up, float3& right) const;

	unsigned _textureId = 0;
	
};

