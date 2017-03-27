#pragma once
#include "BaseComponent.h"
#include "CameraComponent.h"

class BillboardComponent :
	public BaseComponent
{
public:
	BillboardComponent();
	~BillboardComponent();

	void Update() override;
	void DrawUI() override;

	void SetTexture(unsigned textureId);

private:
	unsigned _texture = 0;
	float _width, _height;

	void ComputeQuad(const CameraComponent& camera, float3& up, float3& right) const;
};

