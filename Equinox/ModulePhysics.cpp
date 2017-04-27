#include "btBulletDynamicsCommon.h"
#include "ModulePhysics.h"

ModulePhysics::ModulePhysics()
{
}


ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Init()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
#pragma push_macro("new") // Bullet uses optimized new definitions, we need to remove the new redefinition in debug mode
#undef new
	solver = new btSequentialImpulseConstraintSolver;
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
#pragma pop_macro("new")
	world->setGravity(btVector3(0.0f, -10.0f, 0.0f));
	return true;
}

update_status ModulePhysics::PreUpdate(float DeltaTime)
{
	world->stepSimulation(DeltaTime, 15);
	return UPDATE_CONTINUE;
}
