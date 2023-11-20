#pragma once

#include "Particle.h"

struct ParticleSystemInfo
{
	ParticleInfo particleInfo;
	FoxEngine::FoxMath::Vector3 spawnPos;
	FoxEngine::FoxMath::Vector3 spawnDir;
	float spawnDelay;

	int minParticlesPerEmit = 0;
	int maxParticlesPerEmit = 0;

	float minTimeBetweenParticles = 0.0f;
	float maxTimeBetweenParticles = 0.0f;
	float minSpawnAngles = 0.0f;
	float maxSpawnAngles = 0.0f;
	float lifeTime = 0.0f;
	int maxParticles = 100;
};

class ParticleSystem
{
public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

	void Initialize(const ParticleSystemInfo& info);
	void Terminate();
	void Update(float deltaTime);
	void DebugUI();

	template <class Effect>
	void Render(Effect& effect)
	{
		if (mLifeTime > 0.0f)
		{
			for (auto& particle : mParticles)
			{
				particle.Render(effect);
			}
		}
	}
private:

	void SpawnParticles();
	void SpawnParticle();

	using Particles = std::vector<Particle>;
	Particles mParticles;

	int mNextAvailableParticleIndex;
	float mLifeTime = 0.0f;
	float mNextSpawn = 0.0f;

	ParticleSystemInfo mInfo;
};