#include "TransformComponent.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float4x4.h>
#include "IMGUI/imgui.h"
#include <MathGeoLib/include/Math/float3x3.h>
#include "GameObject.h"

TransformComponent::TransformComponent()
{
	Name = "Transform";
}

TransformComponent::~TransformComponent()
{
}

const float4x4& TransformComponent::GetTransformMatrix()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return _transformMatrix;
}

void TransformComponent::SetTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_transformMatrix = matrix;
	recalculateLocalTransform();
	_dirty = false;
}

const float4x4& TransformComponent::GetLocalTransformMatrix()
{
	return _localTransformMatrix;
}

void TransformComponent::SetLocalTransformMatrix(const float4x4& matrix)
{
	markChildrenDirty();
	_localTransformMatrix = matrix;
	recalculateTransform();
	_dirty = false;
}

void TransformComponent::Update(float dt)
{	
	float4x4 transformMatrix = GetTransformMatrix().Transposed();
	glMultMatrixf(reinterpret_cast<GLfloat*>(&transformMatrix));
}

void TransformComponent::EditorUpdate(float dt)
{
	Update(dt);
}

void TransformComponent::DrawUI()
{
	float3 position = GetLocalPosition();
	if (ImGui::InputFloat3("Position", &position[0], -1, ImGuiInputTextFlags_CharsDecimal))
	{
		SetLocalPosition(position);
	}
	float3 rot = GetLocalRotation().ToEulerXYZ() * RadToDeg(GetLocalRotation().Angle());
	ImGui::SliderFloat3("Rotation", &rot[0], -360, 360, "%.2f deg");
	float3 scale = GetLocalScale();
	if (ImGui::InputFloat3("Scale", &scale[0], -1, ImGuiInputTextFlags_CharsDecimal))
	{
		SetLocalScale(scale);
	}
}

float3 TransformComponent::GetPosition()
{
	if (_dirty)
	{
		_dirty = false;
		recalculateTransform();
	}

	return _transformMatrix.TranslatePart();
}

void TransformComponent::SetPosition(const float3& position)
{
	if (!position.Equals(GetPosition()))
	{
		markChildrenDirty();
		_transformMatrix.SetTranslatePart(position);
		_dirty = false;
		recalculateLocalTransform();
	}
}

float3 TransformComponent::GetLocalPosition() const
{
	return _localTransformMatrix.TranslatePart();
}

void TransformComponent::SetLocalPosition(const float3& position)
{
	if (!position.Equals(GetLocalPosition()))
	{
		markChildrenDirty();
		_localTransformMatrix.SetTranslatePart(position);
		_dirty = false;
		recalculateTransform();
	}
}

Quat TransformComponent::GetRotation()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}

	return Quat(_transformMatrix.RotatePart());
}

void TransformComponent::SetRotation(const Quat& rotation)
{
	if (!rotation.Equals(GetRotation()))
	{
		markChildrenDirty();
		_transformMatrix.SetRotatePart(rotation);
		_dirty = false;
		recalculateLocalTransform();
	}
}

Quat TransformComponent::GetLocalRotation() const
{
	return Quat(_localTransformMatrix.RotatePart());
}

void TransformComponent::SetLocalRotation(const Quat& rotation)
{
	if (!rotation.Equals(GetLocalRotation()))
	{
		markChildrenDirty();
		_localTransformMatrix.SetRotatePart(rotation);
		_dirty = false;
		recalculateTransform();
	}
}

float3 TransformComponent::GetScale()
{
	if (_dirty)
	{
		recalculateTransform();
		_dirty = false;
	}
	return _transformMatrix.ExtractScale();
}

void TransformComponent::SetScale(const float3& scale)
{
	if (!scale.Equals(GetScale()))
	{
		markChildrenDirty();
		_transformMatrix = float4x4::FromTRS(_transformMatrix.TranslatePart(), 
			_transformMatrix.RotatePart(), scale);
		_dirty = false;
		recalculateLocalTransform();
	}
}

float3 TransformComponent::GetLocalScale() const
{
	return _localTransformMatrix.ExtractScale();
}

void TransformComponent::SetLocalScale(const float3& scale)
{
	if (!scale.Equals(GetLocalScale()))
	{
		markChildrenDirty();
		_localTransformMatrix = float4x4::FromTRS(_localTransformMatrix.TranslatePart(), 
			_localTransformMatrix.RotatePart(), scale);
		_dirty = false;
		recalculateTransform();
	}
}

void TransformComponent::markChildrenDirty()
{
	_dirty = true;
	if (Parent)
	{
		for (GameObject* child : Parent->GetChilds())
			child->GetTransform()->markChildrenDirty();
	}
}

void TransformComponent::recalculateTransform()
{	
	_transformMatrix = _localTransformMatrix;

	GameObject* parent = Parent->GetParent();
	if (parent && parent->GetTransform())
		_transformMatrix += parent->GetTransform()->GetTransformMatrix();
}

void TransformComponent::recalculateLocalTransform()
{
	_localTransformMatrix = _transformMatrix;
	
	GameObject* parent = Parent->GetParent();
	if (parent && parent->GetTransform())
		_localTransformMatrix -= parent->GetTransform()->GetTransformMatrix();
}
