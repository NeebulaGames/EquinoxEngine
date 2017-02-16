#include "ModuleEditor.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl_gl3.h"
#include "Engine.h"
#include "ModuleWindow.h"

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
	ImGui::Begin("Debug");

	ImGui::Text("Hello, world %d", 123);

	if (ImGui::Button("Ok"))
	{
		
	}

	char buf[200];

	ImGui::InputText("string", buf, 256);

	float f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

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
