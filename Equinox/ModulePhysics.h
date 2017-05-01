#ifndef __MODULEPHYSICS_H__
#define __MODULEPHYSICS_H__

#include "Module.h"

class ModulePhysics :
	public Module
{
public:
	ModulePhysics();
	~ModulePhysics();

	btRigidBody* AddBody(float boxSize);

	bool Init() override;
	update_status PreUpdate(float DeltaTime) override;
	bool CleanUp() override;

private:

	class DebugDrawer* _debug_drawer = nullptr;

	class btDefaultCollisionConfiguration* _collision_conf = nullptr;
	class btCollisionDispatcher* _dispatcher = nullptr;
	class btBroadphaseInterface* _broad_phase = nullptr;
	class btSequentialImpulseConstraintSolver* _solver = nullptr;
	class btDiscreteDynamicsWorld* _world = nullptr;
};

#endif

