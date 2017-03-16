#include "MaterialComponent.h"
#include "IMGUI/imgui.h"

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

void MaterialComponent::DrawUI()
{
	for (int i = 0; i < Materials.size(); ++i)
	{
		char name[20];
		sprintf_s(name, "Material %i", i);
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlapMode;
		if (ImGui::TreeNodeEx(name, flags))
		{
			Material* mat = Materials[i];
			
			char* path = mat->FilePath;
			if (strcmp(path, "") == 0)
				path = "(None)";

			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(240, 230, 140));
			ImGui::LabelText("", "Texture: %s", path);
			ImGui::PopStyleColor();

			ImGui::SliderFloat4("Ambient", &mat->ambient.r, 0.f, 1.f);
			ImGui::SliderFloat4("Diffuse", &mat->diffuse.r, 0.f, 1.f);
			ImGui::SliderFloat4("Specular", &mat->specular.r, 0.f, 1.f);
			ImGui::SliderFloat("Shininess", &mat->shininess, 0.f, 128.f);

			ImGui::TreePop();
		}
	}
}
