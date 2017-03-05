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
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, Material->texture);

	glBindBuffer(GL_ARRAY_BUFFER, Material->textureID);
	glTexCoordPointer(3, GL_FLOAT, 0, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
