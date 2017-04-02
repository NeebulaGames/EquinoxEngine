#ifndef __BASECOMPONENT_H__
#define __BASECOMPONENT_H__
#include "GameObject.h"


class BaseComponent
{

public:
	std::string Name = "BaseComponent";
	bool Enabled = true;
	GameObject* Parent = nullptr;

public:
	BaseComponent() {};
	
	virtual ~BaseComponent() {};

	virtual void Update(float dt) {};

	virtual void EditorUpdate(float dt) {};

	virtual void DrawUI() {};

	virtual void CleanUp() {};
	
};

#endif
