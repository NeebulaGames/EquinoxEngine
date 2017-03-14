#include "MeshComponent.h"
#include <GL/glew.h>
#include "IMGUI/imgui.h"
#include "Engine.h"
#include "ModuleEditor.h"

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

	if (App->editor->SelectedGameObject == Parent)
		Parent->DrawBoundingBox();

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void MeshComponent::DrawUI()
{
	ImGui::Checkbox("Enabled", &Enabled); /*ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(255, 0, 0));
	if (ImGui::Button("Delete Component"))
		Parent->DeleteComponent(this);
	ImGui::PopStyleColor();*/

	ImGui::PushStyleColor(ImGuiCol_Text, ImColor(240, 230, 140));

	ImGui::LabelText("", "%i mesh(es)", Meshes.size());
	int vertex, indices;
	vertex = indices = 0;
	int i = 0;
	for (Mesh* mesh : Meshes)
	{
		vertex += mesh->num_vertices;
		indices += mesh->num_indices;
		ImGui::LabelText("", "Mesh %i: %i triangles (%i indices, %i vertices)", i, mesh->num_indices / 3, mesh->num_indices, mesh->num_vertices);
		++i;
	}
	
	ImGui::LabelText("", "Total: %i triangles (%i indices, %i vertices)", indices / 3, indices, vertex);

	ImGui::PopStyleColor();
}
