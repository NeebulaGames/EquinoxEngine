#ifndef __BOXCOLLIDERCOMPONENT_H__
#define __BOXCOLLIDERCOMPONENT_H__

#include "BaseComponent.h"
#pragma push_macro("new")
#undef new
#include <LinearMath/btMotionState.h>
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

	void SetSize(float x, float y, float z);
	void SetSize(const float3& halfSize);
	float3 GetGravity() const;
	void SetGravity(const float3& gravity);

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

	float3 _gravity;
	class btRigidBody* _rigidBody = nullptr;
};

#endif // __BOXCOLLIDERCOMPONENT_H__
