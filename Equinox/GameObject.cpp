#include "GameObject.h"
#include "BaseComponent.h"
#include "Globals.h"
#include <GL/glew.h>

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetParent(GameObject* new_parent)
{
	_parent = new_parent;
}

GameObject* GameObject::GetParent()
{
	return _parent;
}

std::vector<GameObject*> GameObject::GetChilds()
{
	return _childs;
}

void GameObject::AddChild(GameObject* child)
{
	if (child != nullptr)
	{
		_childs.push_back(child);
	}
}

std::vector<BaseComponent*> GameObject::GetComponents()
{
	return _components;
}

void GameObject::AddComponent(BaseComponent* component)
{
	if (component != nullptr)
		_components.push_back(component);
}

void GameObject::DeleteComponentByName(std::string name)
{
	//TODO:
}

void GameObject::Update()
{
	glPushMatrix();
	for (BaseComponent* baseComponent : _components)
	{
		baseComponent->Update();
	}
	glPopMatrix();
}

