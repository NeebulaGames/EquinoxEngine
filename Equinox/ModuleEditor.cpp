#include "ModuleEditor.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl_gl3.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include <GL/glew.h>
#include <iterator>

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

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);
	ImVec2 windowPosition = ImGui::GetWindowSize();
	windowPosition.y = 0;
	ImGui::SetNextWindowPos(windowPosition, ImGuiSetCond_Always);
	if (ImGui::Begin("Engine Stats", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{

		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		_fpsValues.push_back(ImGui::GetIO().Framerate);

		if (_fpsValues.size() > 30)
			_fpsValues.pop_front();

		if (ImGui::BeginChild("Histogram", ImVec2(0, 0), true))
		{
			ImGui::PlotHistogram("Framerate", &ModuleEditor::ListGetter, &_fpsValues, _fpsValues.size(), 0, nullptr, 0, 120);
		}

		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);
	windowPosition.y = 110;
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
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
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
