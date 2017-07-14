#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/Quat.h>
#include <MathGeoLib/include/Math/float4x4.h>

class TransformComponent :
	public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION;

public:
	TransformComponent();
	~TransformComponent();

	const float4x4& GetTransformMatrix();
	void SetTransformMatrix(const float4x4& matrix);

	const float4x4& GetLocalTransformMatrix();
	void SetLocalTransformMatrix(const float4x4& matrix);

	void Update(float dt) override;
	void EditorUpdate(float dt) override;
	void DrawUI() override;

	float3 GetPosition();
	void SetPosition(const float3& position);
	float3 GetLocalPosition() const;
	void SetLocalPosition(const float3& position);

	Quat GetRotation();
	void SetRotation(const Quat& rotation);
	Quat GetLocalRotation() const;
	void SetLocalRotation(const Quat& rotation);

	float3 GetLocalScale() const;
	void SetLocalScale(const float3& scale);

private:
	void markChildrenDirty();
	void regenerateLocalTransform();
	void recalculateTransform();
	void recalculateLocalTransform();

	bool _dirty = false;

	float3 _localPosition = float3::zero;
	Quat _localRotation = Quat::identity;
	float3 _localScale = float3::one;

	float4x4 _localTransformMatrix = float4x4::identity;
	float4x4 _transformMatrix = float4x4::identity;
};

#endif
