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
	float3 gravity = _gravity;
	ImGui::InputFloat3("Gravity", &gravity[0], -1, ImGuiInputTextFlags_CharsDecimal);

	if (!gravity.Equals(_gravity))
		SetGravity(gravity);
}

void RigidBodyComponent::getWorldTransform(btTransform& worldTrans) const
{
	float4x4 transform = Parent->GetTransform()->GetTransformMatrix();
	float3 translate = transform.TranslatePart();
	Quat rot = transform.RotatePart().ToQuat();
	worldTrans.setOrigin(btVector3(translate.x, translate.y, translate.z));
	worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}

void RigidBodyComponent::setWorldTransform(const btTransform& worldTrans)
{
	btQuaternion rot = worldTrans.getRotation();
	btVector3 pos = worldTrans.getOrigin();

	float4x4 new_global(Quat(rot.x(), rot.y(), rot.z(), rot.w()), float3(pos.x(), pos.y(), pos.z()));

	// now find out our new local transformation in order to meet the global one from physics
	float4x4 new_local = new_global * Parent->GetParent()->GetTransform()->GetTransformMatrix().Inverted();
	float3 translation, scale;
	Quat rotation;

	new_local.Decompose(translation, rotation, scale);
	Parent->GetTransform()->Position = translation;
	Parent->GetTransform()->Rotation = rotation;
	Parent->GetTransform()->Scale = scale;
}

void RigidBodyComponent::createBody()
{
	_rigidBody = App->physics->AddBody(_box, this);
	_rigidBody->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}

void RigidBodyComponent::SetSize(float x, float y, float z)
{
	SetSize(float3(x, y, z));
}

void RigidBodyComponent::SetSize(const float3& halfSize)
{
	_box = halfSize;
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
