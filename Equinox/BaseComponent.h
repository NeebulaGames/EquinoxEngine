#ifndef __BASECOMPONENT_H__
#define __BASECOMPONENT_H__
#include "Globals.h"

#define DEFAULT_COMPONENT_IMPLEMENTATION \
	public: \
		inline size_t Size() const override { return sizeof(*this); } \
	private: \

class GameObject;

class BaseComponent
{
	friend class GameObject;
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

	virtual void CleanUp()
	{
		if (backup)
			RELEASE(backup);
	};

	virtual inline size_t Size() const = 0;

private:
	static void* CreateBackup(BaseComponent* obj)
	{
		char* b = new char[obj->Size()];
		memcpy(b, obj, obj->Size());
		obj->backup = b;
		return obj->backup;
	}

	static void RestoreBackup(BaseComponent* obj)
	{
		char* b = obj->backup;
		memcpy(obj, obj->backup, obj->Size());
		RELEASE(b);
		obj->backup = nullptr;
	}

	char* backup = nullptr;
};

#endif
