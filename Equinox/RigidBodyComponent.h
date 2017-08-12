#ifndef __BOXCOLLIDERCOMPONENT_H__
#define __BOXCOLLIDERCOMPONENT_H__

#include "BaseComponent.h"
#pragma push_macro("new")
#undef new
#include <LinearMath/btMotionState.h>
#include "Point3.h"
#pragma pop_macro("new")

class RigidBodyComponent :
	public BaseComponent, btMotionState
{
	DEFAULT_COMPONENT_IMPLEMENTATION;

	enum ColliderShape : int
	{
		BOX = 0,
		SPHERE,
		CAPSULE,
		CYLINDER
	};

public:
	RigidBodyComponent();
	~RigidBodyComponent();

	float GetMass() const;
	void SetMass(float size);
	void SetSize(float x, float y, float z);
	void SetSize(const float3& halfSize);
	float3 GetGravity() const;
	void SetGravity(const float3& gravity);
	void SetKinematic(bool kinematic);
	bool IsKinematic() const;
	float3 GetLinearFactor() const;
	void SetLinearFactor(const float3& factor);
	float3 GetAngularFactor() const;
	void SetAngularFactor(const float3& factor);

	void Attached() override;
	void BeginPlay() override;
	void Update(float dt) override;
	void EndPlay() override;
	void CleanUp() override;
	void DrawUI() override;

	void getWorldTransform(btTransform& worldTrans) const override;
	void setWorldTransform(const btTransform& worldTrans) override;

private:
	void createBody();

	ColliderShape _shape = BOX;
	float3 _colliderConfig;
	bool _isKinematic = false;
	float3 _linearFactor = float3::one;
	float3 _angularFactor = float3::one;

	float _mass = 1.f;
	float3 _gravity;
	float3 _center;
	class btRigidBody* _rigidBody = nullptr;
};

#endif // __BOXCOLLIDERCOMPONENT_H__
