#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float2.h>
#include "IMGUI/imgui.h"

class CameraComponent;

struct Particle
{
	float3 Position;
	float3 Velocity;
	float LifeTime;
	bool IsAlive = false;
};

class ParticleEmitter : public BaseComponent
{
	DEFAULT_COMPONENT_IMPLEMENTATION;
public:
	ParticleEmitter(int MaxParticles, float2 EmitArea, float FallHeight, float FallSpeed, float LifeTime);
	~ParticleEmitter();
	
	void Update(float dt) override;
	void EditorUpdate(float dt) override;
	void DrawUI() override;
	void CleanUp() override;

	void SetTexture(unsigned textureId);

	std::vector<Particle*> ParticlePool;
	float2 EmitArea;
	int MaxParticles;

	float FallHeight;
	float FallSpeed;
	float LifeTime;

private:

	unsigned _texture = 0;
	float _width, _height;

	bool _editorSimulation = false;

	void drawParticle(Particle* particle);
	void generateParticles();
	void restart();
	void checkValues();

	void ComputeQuad(const CameraComponent& camera, float3& up, float3& right, Particle* particle) const;

	float2 _controlEmitArea;
	int _controlMaxParticles;

	float _controlFallHeight;
	float _controlFallSpeed;
	float _controlLifeTime;
};

#endif
