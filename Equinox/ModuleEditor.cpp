#include "ModuleEditor.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl_gl3.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include <GL/glew.h>

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
	ImGui::Begin("Engine Stats", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

	bool wireframe = _wireframe;
	ImGui::Checkbox("Wireframe mode", &wireframe);

	if (wireframe != _wireframe)
	{
		_wireframe = wireframe;
		wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
