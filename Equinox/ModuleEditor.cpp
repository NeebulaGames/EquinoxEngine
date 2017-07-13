#include "ModuleEditor.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl_gl3.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include <GL/glew.h>
#include <iterator>
#include "BaseComponent.h"
#include "ModuleLighting.h"
#include <Brofiler.h>

ModuleEditor::ModuleEditor() : Module()
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	ImGui_ImplSdlGL3_Init(App->window->window);
	return true;
}

update_status ModuleEditor::PreUpdate(float DeltaTime)
{ BROFILER_CATEGORY("PreUpdateEditor", Profiler::Color::AliceBlue)
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float DeltaTime)
{ BROFILER_CATEGORY("UpdateEditor", Profiler::Color::AntiqueWhite)
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	ImGui::SetNextWindowSize(ImVec2(120, 10), ImGuiSetCond_Always);
	ImVec2 playPosition(w / 2 - 60, 10);
	ImGui::SetNextWindowPos(playPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Editor Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoResize | ImGuiAlign_Center))
	{
		
		if (ImGui::Button(_isPlaying ? "Stop" : "Play"))
		{
			_isPlaying = !_isPlaying; // TODO: Convert this to a state machine
		}

		ImGui::SameLine();

		if (ImGui::Button(_isPaused ? "Unpause" : "Pause"))
		{
			_isPaused = !_isPaused;
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_Always);
	ImVec2 windowPosition(0, h - 100);
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Engine Stats", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{
		float framerate = ImGui::GetIO().Framerate;
		
		_fpsValues.push_back(framerate);
		if (_fpsValues.size() > 30)
			_fpsValues.pop_front();

		if (ImGui::BeginChild("Histogram", ImVec2(0, 0), true))
		{
			ImGui::Text("FPS: %f", framerate);
			ImGui::PlotHistogram("Framerate", &ModuleEditor::ListGetter, &_fpsValues, _fpsValues.size(), 0, nullptr, 0, 120);
		}

		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
	windowPosition.x = 301;
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Engine Debug", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{

		bool wireframe = _wireframe;
		ImGui::Checkbox("Wireframe mode", &wireframe);

		if (wireframe != _wireframe)
		{
			_wireframe = wireframe;
			wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		ImGui::Checkbox("Draw hierachy", &DrawHierachy);
		ImGui::Checkbox("Draw quadtree", &DrawQuadtree);
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(400, h-400), ImGuiSetCond_Always);
	windowPosition.y = 0;
	windowPosition.x = w - 400;
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (SelectedGameObject)
		{
			for (BaseComponent* component : SelectedGameObject->GetComponents())
			{
				if (ImGui::CollapsingHeader(component->Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlapMode))
					component->DrawUI();
			}
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_Always);
	windowPosition.y = h - 400;
	windowPosition.x = w - 400;
	
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Lighting", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ModuleLighting* lighting = App->lighting;

		if (ImGui::CollapsingHeader("Global Ambient Light", ImGuiTreeNodeFlags_AllowOverlapMode))
		{
			ImGui::Checkbox("Enabled", &lighting->EnableAmbientLight);

			if (lighting->EnableAmbientLight)
			{
				ImGui::InputFloat4("Ambient", &lighting->AmbientLight[0], -1, ImGuiInputTextFlags_CharsDecimal);
			}
		}

		int i = 0;
		for (Light* light : lighting->Lights)
		{
			std::string name = "Light-" + std::to_string(i);
			if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_AllowOverlapMode))
			{
				ImGui::Checkbox("Enabled", &light->IsEnabled);

				if(light->IsEnabled)
				{
					ImGui::InputFloat4("Ambient", &light->Ambient[0], -1, ImGuiInputTextFlags_CharsDecimal);
					ImGui::InputFloat4("Diffuse", &light->Diffuse[0], -1, ImGuiInputTextFlags_CharsDecimal);
					ImGui::InputFloat4("Specular", &light->Specular[0], -1, ImGuiInputTextFlags_CharsDecimal);

					ImGui::NewLine();
					
					if (light->Type == L_POINT || light->Type == L_DIRECTIONAL || light->Type == L_SPOTLIGHT)
					{
						ImGui::InputFloat3("Position", &light->Position[0], -1, ImGuiInputTextFlags_CharsDecimal);
					}
					if (light->Type == L_SPOTLIGHT)
					{
						ImGui::InputFloat("CutOff", &light->CutOff, -1, ImGuiInputTextFlags_CharsDecimal);
						ImGui::InputFloat3("Direction", &light->Direction[0], -1, ImGuiInputTextFlags_CharsDecimal);
					}

					int current_type = lighting->GetLabelByType(light->Type);
					ImGui::Combo("Type", &current_type, "Point\0Directional\0Spotlight\0Default\0");
					if (lighting->GetTypeByLabel(current_type) != light->Type)
						lighting->SetLightType(light, lighting->GetTypeByLabel(current_type));
				}
			}
			i++;
		}
		
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float DeltaTime)
{ BROFILER_CATEGORY("PostUpdateEditor", Profiler::Color::Aqua)
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

float ModuleEditor::ListGetter(void* data, int id)
{
	auto it = reinterpret_cast<std::list<float>*>(data)->begin();
	advance(it, id);
	return *it;
}
