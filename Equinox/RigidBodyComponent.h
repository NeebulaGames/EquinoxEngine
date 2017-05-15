#ifndef __BOXCOLLIDERCOMPONENT_H__
#define __BOXCOLLIDERCOMPONENT_H__

#include "BaseComponent.h"
#include <LinearMath/btMotionState.h>

class RigidBodyComponent :
	public BaseComponent, btMotionState
{
	DEFAULT_COMPONENT_IMPLEMENTATION;

public:
	RigidBodyComponent();
	~RigidBodyComponent();

	void SetSize(float x, float y, float z);
	void SetSize(float3 halfSize);
	void CreateBody();

	void Attached() override;
	void BeginPlay() override;
	void Update(float dt) override;
	void EndPlay() override;
	void CleanUp() override;

	void getWorldTransform(btTransform& worldTrans) const override;
	void setWorldTransform(const btTransform& worldTrans) override;

private:
	bool _defined = false;
	float3 _box;
	class btRigidBody* _rigidBody = nullptr;
};

#endif // __BOXCOLLIDERCOMPONENT_H__
