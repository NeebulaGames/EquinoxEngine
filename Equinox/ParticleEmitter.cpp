#include "ParticleEmitter.h"
#include <GL/glew.h>
#include "Globals.h"
#include "IMGUI/imgui.h"
#include "Engine.h"
#include "ModuleEditor.h"

ParticleEmitter::ParticleEmitter(int MaxParticles, float2 EmitArea, float FallHeight, float FallSpeed, float LifeTime)
{
	Name = "Particle System";

	this->MaxParticles = MaxParticles;
	this->EmitArea = EmitArea;
	this->FallHeight = FallHeight;
	this->FallSpeed = FallSpeed;
	this->LifeTime = LifeTime;
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(float dt)
{
	GenerateParticles();

	for (Particle* particle : ParticlePool)
	{
		if (particle->IsAlive)
		{
			DrawParticle(particle);

			// Update particle position & time;
			particle->Position = particle->Position + particle->Velocity * dt;
			particle->LifeTime -= dt;

			// When lifetime finish, kill particle
			if (particle->LifeTime < dt)
			{
				particle->IsAlive = false;
			}
		}
	}
}

void ParticleEmitter::EditorUpdate(float dt)
{
	if (_editorSimulation && !App->editor->IsPlaying())
		Update(dt);
}

void ParticleEmitter::DrawUI()
{
	ImGui::Checkbox("Simulate On Editor", &_editorSimulation);

	ImGui::InputFloat2("Emit Area", &EmitArea[0], -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputInt("Max Particles", &MaxParticles, -1);
	ImGui::InputFloat("Fall Height", &FallHeight, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Fall Speed", &FallSpeed, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Particle's LifeTime", &LifeTime, -1, ImGuiInputTextFlags_CharsDecimal);
}

void ParticleEmitter::CleanUp()
{
	for (Particle* particle : ParticlePool)
	{
		RELEASE(particle);
	}
}

void ParticleEmitter::DrawParticle(Particle* particle)
{
	// Enable for billboards
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glDepthMask(GL_FALSE);

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw just points for now.
	glPushMatrix();

	glBegin(GL_POINTS);
	glVertex3fv(reinterpret_cast<const GLfloat*>(&particle->Position));
	glEnd();

	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
}

void ParticleEmitter::GenerateParticles()
{
	int particlesToGen = MaxParticles / LifeTime;

	for (int i = 0; i < particlesToGen; ++i)
	{
		Particle* particle = new Particle;

		particle->IsAlive = true;

		float rand_x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / EmitArea.x));
		float rand_z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / EmitArea.y));

		particle->Position = float3(rand_x, FallHeight, rand_z);
		particle->Velocity = float3(0.0f, -FallSpeed, 0.0f);

		particle->LifeTime = LifeTime;

		ParticlePool.push_back(particle);
	}


}
