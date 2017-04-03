#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float2.h>

struct Particle
{
	float3 Position;
	float3 Velocity;
	float LifeTime;
	unsigned Texture;
	bool IsAlive = false;
};

class ParticleEmitter : public BaseComponent
{
public:
	ParticleEmitter(float2 EmitArea);
	ParticleEmitter(int NumParticles, float2 EmitArea);
	~ParticleEmitter();

	void Init();

	void Update() override;
	void CleanUp() override;
	void DrawParticle(Particle* particle);

	std::vector<Particle*> ParticlePool;
	float2 EmitArea;
	int NumParticles;

	float FallHeight;
	float LifeTime;
};

#endif
