#ifndef __BASECOMPONENT_H__
#define __BASECOMPONENT_H__
#include "Globals.h"

class GameObject;

class BaseComponent
{

public:
	std::string Name = "BaseComponent";
	bool Enabled = true;
	GameObject* Parent = nullptr;

public:
	BaseComponent() {};
	
	virtual ~BaseComponent() {};

	virtual void BeginPlay() {}

	virtual void Update(float dt) {};

	virtual void EditorUpdate(float dt) {};

	virtual void EndPlay() {}

	virtual void DrawUI() {};

	virtual void CleanUp() {};

protected:
	template<class T>
	void* CreateBackup(T* obj)
	{
		char* b = new char[sizeof(T)];
		memcpy(b, this, sizeof(T));
		backup = b;
		return backup;
	}

	template<class T>
	void RestoreBackup(T* obj)
	{
		char* b = backup;
		memcpy(obj, backup, sizeof(T));
		RELEASE(b);
		backup = nullptr;
	}

private:
	char* backup = nullptr;
};

#endif
