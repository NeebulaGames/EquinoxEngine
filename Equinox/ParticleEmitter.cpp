#include "ParticleEmitter.h"
#include <GL/glew.h>
#include "Globals.h"

ParticleEmitter::ParticleEmitter(float2 EmitArea)
{
	NumParticles = 100;
	this->EmitArea = EmitArea;
	this->FallHeight = 30.f;
	Init();
}

ParticleEmitter::ParticleEmitter(int NumParticles, float2 EmitArea)
{
	this->NumParticles = NumParticles;
	this->EmitArea = EmitArea;
	this->FallHeight = 30.f;

	Init();
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Init()
{
	for (int i = 0; i < NumParticles; ++i)
		ParticlePool.push_back(new Particle);
}

void ParticleEmitter::Update()
{
	for (Particle* particle : ParticlePool)
	{
		if (particle->IsAlive)
		{
			DrawParticle(particle);
			// Update particle position;
			particle->Position = particle->Position + particle->Velocity;

			// When lifetime finish, kill particle / for now limit with height
			if (particle->Position.y <= 0)
			{
				particle->IsAlive = false;
			}
		}
		else
		{
			float rand_x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / EmitArea.x));
			float rand_z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / EmitArea.y));

			particle->Position = float3(rand_x, FallHeight, rand_z);
			// fixed speed for testing
			particle->Velocity = float3(0.0f, -0.51f, 0.0f);

			particle->LifeTime = LifeTime;
			particle->IsAlive = true;
		}
	}
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
