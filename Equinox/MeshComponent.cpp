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
	if (Parent->VisibleOnCamera)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		for (Mesh* mesh : Meshes)
		{
			glColor3f(1.f, 1.f, 1.f);
			Material* mat = MaterialComponent->Materials[mesh->materialInComponent];

			glMaterialfv(GL_FRONT, GL_AMBIENT, reinterpret_cast<GLfloat*>(&mat->ambient));
			glMaterialfv(GL_FRONT, GL_DIFFUSE, reinterpret_cast<GLfloat*>(&mat->diffuse));
			glMaterialfv(GL_FRONT, GL_SPECULAR, reinterpret_cast<GLfloat*>(&mat->specular));
			glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

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

			glBindTexture(GL_TEXTURE_2D, mat->texture);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);

			glMaterialfv(GL_FRONT, GL_AMBIENT, DEFAULT_GL_AMBIENT);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, DEFAULT_GL_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_SPECULAR, DEFAULT_GL_SPECULAR);
			glMaterialf(GL_FRONT, GL_SHININESS, DEFAULT_GL_SHININESS);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		if (App->editor->SelectedGameObject == Parent)
			Parent->DrawBoundingBox();

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
	}
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
