#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <vector>
#include <string>

class BaseComponent;

class GameObject
{

public:
	std::string Name = "GameObject";
	bool Enabled = true;
	
private:
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _childs;
	std::vector<BaseComponent*> _components;

public:
	GameObject();
	~GameObject();

	void SetParent(GameObject* new_parent);
	GameObject* GetParent() const;

	const std::vector<GameObject*>& GetChilds() const;
	void AddChild(GameObject* child);
	void RemoveChild(const GameObject* child);

	const std::vector<BaseComponent*>& GetComponents() const;
	void AddComponent(BaseComponent* component);
	BaseComponent* GetComponentByName(const std::string& name) const;
	void DeleteComponentByName(const std::string& name);
	
	void Update();

	bool CleanUp();

};

#endif
