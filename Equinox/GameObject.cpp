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
	if(_parent != nullptr)
	{
		_parent->RemoveChild(this);
		new_parent->_childs.push_back(this);
		_parent = new_parent;
	}
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
		if(child->_parent != nullptr)
			child->_parent->RemoveChild(this);
		child->_parent = this;
		_childs.push_back(child);
	}
}

void GameObject::RemoveChild(GameObject* child)
{
	if(!_childs.empty())
	{
		for (auto it = _childs.begin(); it != _childs.cend(); ++it)
		{
			if (*it == child)
			{
				_childs.erase(it);
				break;
			}
		}
	}
}

std::vector<BaseComponent*> GameObject::GetComponents()
{
	return _components;
}

void GameObject::AddComponent(BaseComponent* component)
{
	if (component != nullptr)
	{
		component->Parent = this;
		_components.push_back(component);
	}
}

void GameObject::DeleteComponentByName(std::string name)
{
	if(!_components.empty())
	{
		for (auto it = _components.begin(); it != _components.cend(); ++it)
		{
			if ((*it)->Name == name)
			{
				_components.erase(it);
			}
		}
	}
}

void GameObject::Update()
{
	glPushMatrix();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (BaseComponent* baseComponent : _components)
	{
		baseComponent->Update();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

