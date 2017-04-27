#ifndef __MODULEPHYSICS_H__
#define __MODULEPHYSICS_H__

#include "Module.h"

class ModulePhysics :
	public Module
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Init() override;
	update_status PreUpdate(float DeltaTime) override;

private:
	class btDefaultCollisionConfiguration* collision_conf = nullptr;
	class btCollisionDispatcher* dispatcher = nullptr;
	class btBroadphaseInterface* broad_phase = nullptr;
	class btSequentialImpulseConstraintSolver* solver = nullptr;
	class btDiscreteDynamicsWorld* world = nullptr;
};

#endif
