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
	DEFAULT_COMPONENT_IMPLEMENTATION;
public:
	ParticleEmitter(int MaxParticles, float2 EmitArea, float FallHeight, float FallSpeed, float LifeTime);
	~ParticleEmitter();
	
	void Update(float dt) override;
	void CleanUp() override;

	std::vector<Particle*> ParticlePool;
	float2 EmitArea;
	int MaxParticles;

	float FallHeight;
	float FallSpeed;
	float LifeTime;

private:

	void DrawParticle(Particle* particle);
	void GenerateParticles();
};

#endif
