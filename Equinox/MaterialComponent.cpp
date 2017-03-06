#include "MaterialComponent.h"

MaterialComponent::MaterialComponent()
{
	Name = "Material";
}

MaterialComponent::~MaterialComponent()
{
}

unsigned MaterialComponent::AddMaterial(Material* material)
{
	int count = 0;
	for (Material* mat : Materials)
	{
		if (mat == material)
			return count;
		++count;
	}

	Materials.push_back(material);
	return count;
}
