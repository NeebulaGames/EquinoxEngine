#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__
#include "BaseComponent.h"
#include <assimp/color4.h>
#include <GL/glew.h>

struct Material
{
	aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	GLuint textureID = 0;
	float shininess = 0.0f;
	unsigned texture = 0;
	char FilePath[256];
};

class MaterialComponent :
	BaseComponent
{
public:
	Material* Material;	
	
public:
	MaterialComponent();
	~MaterialComponent();

	void Update() override;
};

#endif
