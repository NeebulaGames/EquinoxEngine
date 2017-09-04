#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Rectangle3.h"
#include <GL/glew.h>

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class Primitive;
class Level;

class ModuleRender : public Module
{
public:
	
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool Start();
	update_status PreUpdate(float DeltaTime);
	update_status Update(float DeltaTime);
	update_status PostUpdate(float DeltaTime);
	bool CleanUp();

public:
	SDL_GLContext context = nullptr;
		
private:
	std::list<Primitive*> objects;
};

#endif // __MODULERENDER_H__
