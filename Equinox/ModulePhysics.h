#ifndef __MODULEPHYSICS_H__
#define __MODULEPHYSICS_H__

#include "Module.h"
#include <list>

class btRigidBody;
class btMotionState;
class btCollisionShape;

class ModulePhysics :
	public Module
{
public:
	ModulePhysics();
	~ModulePhysics();

	btRigidBody* AddBody(const float3& box, btMotionState* component);
	void RemoveBody(btRigidBody* rigidBody);
	float3 GetGravity() const;

	bool Init() override;
	update_status PreUpdate(float DeltaTime) override;
	bool CleanUp() override;

private:

	std::list<class btCollisionShape*> shapes;

	class DebugDrawer* _debug_drawer = nullptr;

	class btDefaultCollisionConfiguration* _collision_conf = nullptr;
	class btCollisionDispatcher* _dispatcher = nullptr;
	class btBroadphaseInterface* _broad_phase = nullptr;
	class btSequentialImpulseConstraintSolver* _solver = nullptr;
	class btDiscreteDynamicsWorld* _world = nullptr;
};

#endif

