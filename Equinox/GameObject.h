#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <vector>
#include <string>
#include <list>
#include <MathGeoLib/include/Geometry/AABB.h>

class BaseComponent;
class TransformComponent;

class GameObject
{

public:
	std::string Name = "GameObject";
	bool Enabled = true;
	AABB BoundingBox;
	
private:
	GameObject* _parent = nullptr;
	TransformComponent* _transform = nullptr;
	std::vector<GameObject*> _childs;
	std::list<BaseComponent*> _components;

public:
	GameObject();
	~GameObject();

	void SetParent(GameObject* new_parent);
	GameObject* GetParent() const;

	const std::vector<GameObject*>& GetChilds() const;
	void AddChild(GameObject* child);
	void RemoveChild(const GameObject* child);

	const std::list<BaseComponent*>& GetComponents() const;
	void AddComponent(BaseComponent* component);
	BaseComponent* GetComponentByName(const std::string& name) const;
	void DeleteComponentByName(const std::string& name);
	void DeleteComponent(BaseComponent* component);

	TransformComponent* GetTransform() const;

	void DrawBoundingBox();
	void DrawHierachy();
	void DrawHierachy(const float4x4& transformMatrix);

	void Render();
	
	void Update();

	bool CleanUp();

};

#endif
