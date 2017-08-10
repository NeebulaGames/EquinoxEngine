#pragma push_macro("new")
#undef new
#include <BulletDynamics/Dynamics/btRigidBody.h>
#pragma pop_macro("new")
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "Engine.h"
#include "ModulePhysics.h"
#include "TransformComponent.h"
#include <MathGeoLib/include/Math/float4x4.h>
#include <MathGeoLib/include/Math/float3x3.h>
#include "IMGUI/imgui.h"

RigidBodyComponent::RigidBodyComponent()
{
	Name = "BoxCollider";
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Attached()
{
	AABB boundingBox = Parent->BoundingBox;
	vec size = boundingBox.HalfSize();
	_gravity = App->physics->GetGravity();
	_center = boundingBox.CenterPoint();
	SetSize(size);
}

void RigidBodyComponent::BeginPlay()
{
	createBody();
}

void RigidBodyComponent::Update(float dt)
{
}

void RigidBodyComponent::EndPlay()
{
	App->physics->RemoveBody(_rigidBody);
	_rigidBody = nullptr;
}

void RigidBodyComponent::CleanUp()
{
	if (_rigidBody != nullptr)
		App->physics->RemoveBody(_rigidBody);
	_rigidBody = nullptr;
}

void RigidBodyComponent::DrawUI()
{
	int colliderShape = _shape;
	ImGui::Combo("Collider shape", &colliderShape, "Box\0Sphere\0Capsule\0Cylinder\0");
	_shape = static_cast<ColliderShape>(colliderShape);

	switch (_shape)
	{
	case BOX:
	case CYLINDER:
		ImGui::InputFloat3("Size", &_colliderConfig[0], -1, ImGuiInputTextFlags_CharsDecimal);
		break;
	case SPHERE:
		ImGui::InputFloat("Radius", &_colliderConfig[0], -1, ImGuiInputTextFlags_CharsDecimal);
		break;
	case CAPSULE:
		ImGui::InputFloat2("Extents", &_colliderConfig[0], -1, ImGuiInputTextFlags_CharsDecimal);
		break;
	}

	bool kinematic = _isKinematic;
	if (ImGui::Checkbox("Kinematic", &kinematic))
	{
		SetKinematic(kinematic);
	}

	float3 gravity = _gravity;
	ImGui::InputFloat3("Gravity", &gravity[0], -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat3("Center", &_center[0], -1, ImGuiInputTextFlags_CharsDecimal);

	if (!gravity.Equals(_gravity))
		SetGravity(gravity);
}

void RigidBodyComponent::getWorldTransform(btTransform& worldTrans) const
{
	float4x4 transform = Parent->GetTransform()->GetTransformMatrix();
	float3 translate = transform.TranslatePart() + _center;
	Quat rot = transform.RotatePart().ToQuat();
	worldTrans.setOrigin(btVector3(translate.x, translate.y, translate.z));
	worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}

void RigidBodyComponent::setWorldTransform(const btTransform& worldTrans)
{
	btQuaternion rot = worldTrans.getRotation();
	btVector3 pos = worldTrans.getOrigin();

	Quat rotation = Quat(rot.x(), rot.y(), rot.z(), rot.w());
	float4x4 new_global(rotation, float3(pos.x(), pos.y(), pos.z()) - rotation.Mul(_center));
	Parent->GetTransform()->SetTransformMatrix(new_global);
}

void RigidBodyComponent::createBody()
{
	switch (_shape)
	{
	case BOX:
		_rigidBody = App->physics->AddBoxBody(_colliderConfig, this);
		break;
	case SPHERE:
		_rigidBody = App->physics->AddSphereBody(_colliderConfig.x, this);
		break;
	case CAPSULE:
		_rigidBody = App->physics->AddCapsuleBody(_colliderConfig.x, _colliderConfig.y, this);
		break;
	case CYLINDER:
		_rigidBody = App->physics->AddCylinderBody(_colliderConfig, this);
		break;
	}
	
	_rigidBody->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));

	SetKinematic(_isKinematic);
}

void RigidBodyComponent::SetSize(float x, float y, float z)
{
	SetSize(float3(x, y, z));
}

void RigidBodyComponent::SetSize(const float3& colliderSize)
{
	_colliderConfig = colliderSize;
}

float3 RigidBodyComponent::GetGravity() const
{
	return _gravity;
}

void RigidBodyComponent::SetGravity(const float3& gravity)
{
	_gravity = gravity;

	if (_rigidBody)
		_rigidBody->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}

void RigidBodyComponent::SetKinematic(bool kinematic)
{
	if (_rigidBody)
	{
		if (kinematic)
		{
			_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}
		else if (_rigidBody->isKinematicObject())
		{
			_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() ^ btCollisionObject::CF_KINEMATIC_OBJECT);
		}
	}

	_isKinematic = kinematic;
}

bool RigidBodyComponent::IsKinematic() const
{
	return _isKinematic;
}
