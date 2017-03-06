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

	glBindBuffer(GL_ARRAY_BUFFER, Mesh->vertexID);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, Mesh->normalID);
	glNormalPointer(GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->indexesID);
	glDrawElements(GL_TRIANGLES, Mesh->num_indices, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
