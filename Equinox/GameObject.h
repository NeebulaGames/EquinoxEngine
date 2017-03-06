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
	GameObject* GetParent();

	std::vector<GameObject*> GetChilds();
	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	std::vector<BaseComponent*> GetComponents();
	void AddComponent(BaseComponent* component);
	void DeleteComponentByName(std::string name);
	
	void Update();

};

#endif
