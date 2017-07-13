#include "ParticleEmitter.h"
#include <GL/glew.h>
#include "Globals.h"
#include "IMGUI/imgui.h"
#include "Engine.h"
#include "ModuleEditor.h"
#include "CameraComponent.h"
#include "ModuleEditorCamera.h"

ParticleEmitter::ParticleEmitter(int MaxParticles, float2 EmitArea, float FallHeight, float FallSpeed, float LifeTime)
{
	Name = "Particle System";

	this->MaxParticles = MaxParticles;
	this->EmitArea = EmitArea;
	this->FallHeight = FallHeight;
	this->FallSpeed = FallSpeed;
	this->LifeTime = LifeTime;

	_controlMaxParticles = MaxParticles;
	_controlEmitArea = EmitArea;
	_controlFallHeight = FallHeight;
	_controlFallSpeed = FallSpeed;
	_controlLifeTime = LifeTime;

}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(float dt)
{
	checkValues();

	generateParticles();

	for (Particle* particle : ParticlePool)
	{
		if (particle->IsAlive)
		{
			drawParticle(particle);

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
	if (_editorSimulation)
		Update(dt);
}

void ParticleEmitter::EndPlay()
{
	CleanUp();
}

void ParticleEmitter::DrawUI()
{
	ImGui::Checkbox("Simulate On Editor", &_editorSimulation);

	ImGui::SameLine();

	if (ImGui::Button("Restart"))
	{
		CleanUp();
	}

	ImGui::InputFloat2("Emit Area", &EmitArea[0], -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputInt("Max Particles", &MaxParticles, -1);
	ImGui::InputFloat("Fall Height", &FallHeight, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Fall Speed", &FallSpeed, -1, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputFloat("Particle's LifeTime", &LifeTime, -1, ImGuiInputTextFlags_CharsDecimal);
}

void ParticleEmitter::SetTexture(unsigned textureId)
{
	_texture = textureId;

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
	float ratio = _width / _height;
	_width = ratio;
	_height = 1;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleEmitter::drawParticle(Particle* particle)
{
	// Enable for billboards
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_BLEND);

	glColor3f(1.f, 1.f, 1.f);

	float3 up, right;
	ComputeQuad(*App->editorCamera->GetCamera(), up, right, particle);
	//right = float3::unitZ; 

	float3 position = particle->Position;

	float halfX = (_height * 0.5f) / _height;
	float halfY = (_width * 0.5f) / _width;
	float3 vertex1 = (position + up * _height * 0.5f) + (right * _width * 0.5f);
	float3 vertex2 = (position - up * _height * 0.5f) - (right * _width * 0.5f);
	float3 vertex3 = (position - up * _height * 0.5f) + (right * _width * 0.5f);
	float3 vertex4 = (position + up * _height * 0.5f) - (right * _width * 0.5f);

	glBindTexture(GL_TEXTURE_2D, _texture);

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex1));
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex2));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex3));

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex1));
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex4));
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(reinterpret_cast<GLfloat*>(&vertex2));
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
}

void ParticleEmitter::generateParticles()
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

void ParticleEmitter::CleanUp()
{
	for (Particle* particle : ParticlePool)
	{
		RELEASE(particle);
	}

	ParticlePool.clear();
}

void ParticleEmitter::checkValues()
{
	if (_controlMaxParticles != MaxParticles ||
		_controlEmitArea.x != EmitArea.x ||
		_controlEmitArea.y != EmitArea.y ||
		_controlFallHeight != FallHeight ||
		_controlFallSpeed != FallSpeed ||
		_controlLifeTime != LifeTime)
	{
		_controlMaxParticles = MaxParticles;
		_controlEmitArea = EmitArea;
		_controlFallHeight = FallHeight;
		_controlFallSpeed = FallSpeed;
		_controlLifeTime = LifeTime;

		CleanUp();
	}

}

void ParticleEmitter::ComputeQuad(const CameraComponent& camera, float3& up, float3& right, Particle* particle) const
{
	float3 position = particle->Position;
	up = float3::unitY;
	right = (position - camera.Position()).Normalized().Cross(up);
	right.Normalize();
}
