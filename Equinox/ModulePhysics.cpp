#include "btBulletDynamicsCommon.h"
#include "ModulePhysics.h"
#include "DebugDrawer.h"
#include "Engine.h"
#include "ModuleEditor.h"

ModulePhysics::ModulePhysics()
{
}


ModulePhysics::~ModulePhysics()
{
}

btRigidBody* ModulePhysics::AddBody(const float3& box, btMotionState* component)
{
#pragma push_macro("new")
#undef new
	float mass = 1.0f; // 0.0f would create a static or inmutable body

	btCollisionShape* colShape = new btBoxShape(btVector3(box.x, box.y, box.z)); // regular box
	shapes.push_back(colShape);

	btVector3 localInertia(0.f, 0.f, 0.f);
	if (mass != 0.f) 
		colShape->calculateLocalInertia(mass, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, component, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	_world->addRigidBody(body);
	return body;
#pragma pop_macro("new")
}

void ModulePhysics::RemoveBody(btRigidBody* rigidBody)
{
	if (rigidBody)
	{
		shapes.remove(rigidBody->getCollisionShape());
		_world->removeRigidBody(rigidBody);
		//RELEASE(rigidBody);
	}
}

float3 ModulePhysics::GetGravity() const
{
	btVector3 gravity = _world->getGravity();
	return float3(&gravity[0]);
}

bool ModulePhysics::Init()
{
	_collision_conf = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collision_conf);
	_broad_phase = new btDbvtBroadphase();
#pragma push_macro("new") // Bullet uses optimized new definitions, we need to remove the new redefinition in debug mode
#undef new
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broad_phase, _solver, _collision_conf);
#pragma pop_macro("new")
	_world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	_debug_drawer = new DebugDrawer;
	_world->setDebugDrawer(_debug_drawer);

	return true;
}

update_status ModulePhysics::PreUpdate(float DeltaTime)
{
	if (App->editor->IsPlaying())
	{
		_world->debugDrawWorld();
		_world->stepSimulation(DeltaTime, 15);
	}
	return UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	for (auto it = shapes.begin(); it != shapes.end(); ++it)
		RELEASE(*it);

	shapes.clear();

	RELEASE(_debug_drawer);
	RELEASE(_collision_conf);
	RELEASE(_dispatcher);
	RELEASE(_broad_phase);
	RELEASE(_solver);
	RELEASE(_world);

	return true;
}
