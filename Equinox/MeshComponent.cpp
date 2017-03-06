#include "MeshComponent.h"
#include <GL/glew.h>

MeshComponent::MeshComponent()
{
	Name = "Mesh";
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Update()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	for (Mesh* mesh : Meshes)
	{

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);

		if (mesh->normalID != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh->normalID);
			glNormalPointer(GL_FLOAT, 0, nullptr);
		}

		if (mesh->textureCoordsID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh->textureCoordsID);
			glTexCoordPointer(3, GL_FLOAT, 0, nullptr);
		}
		
		Material* mat = MaterialComponent->Materials[mesh->materialInComponent];

		glColor3f(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, mat->texture);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
