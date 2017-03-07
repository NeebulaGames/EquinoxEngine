#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__
#include "BaseComponent.h"
#include <assimp/color4.h>
#include <GL/glew.h>
#include <list>

struct Material
{
	aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	float shininess = 0.0f;
	unsigned texture = 0;
	char FilePath[256];
};

class MaterialComponent :
	public BaseComponent
{
public:
	std::vector<Material*> Materials;
	
public:
	MaterialComponent();
	~MaterialComponent();

	unsigned AddMaterial(Material* material);
};

#endif
