#pragma once
#include "Particle.h"

struct ParticleSystemInfo
{
	ParticleInfo particleInfo;
	FoxEngine::FoxMath::Vector3 spawnPosition;
	FoxEngine::FoxMath::Vector3 spawnDirection;

	float spawnDelay;
	int minParticlesPerEmit = 0;
	int maxParticlesPerEmit = 0;
	float minTimeBetweenParticles = 0.f;
	float maxTimeBetweenParticles = 0.f;
	float minSpawnAngle = 0.f;
	float maxSpawnAngle = 0.f;
	float minSpeed = 0.f;
	float maxSpeed = 0.f;
	float lifeTime = 0.f;
	int maxParticles = 0;
};

class ParticleSystem
{
	void SpawnParticles();
	void SpawnParticle();

	using Particles = std::vector<Particle>;
	Particles mParticles;

	ParticleSystemInfo mInfo;

	int nextAvailableParticleIndex;
	float nextSpawn = 0.f;
	float mLifeTime = 0.f;

public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

	void Initialize(const ParticleSystemInfo& info);
	void Terminate();
	void Update(const float deltaTime);
	void DebugUI();

	template <class Effect>
	void Render(Effect& effect)
	{
		if (mLifeTime > 0.f)
		{
			for (auto& particle : mParticles)
			{
				particle.Render(effect);
			}
		}
	}
};