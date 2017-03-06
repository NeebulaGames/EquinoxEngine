#include "MaterialComponent.h"

MaterialComponent::MaterialComponent()
{
	Name = "Material";
}

MaterialComponent::~MaterialComponent()
{
}

void MaterialComponent::Update()
{
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, Material->texture);

	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordsId);
	glTexCoordPointer(3, GL_FLOAT, 0, nullptr);
}
