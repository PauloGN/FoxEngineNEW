#include "ParticleSystem.h"


using namespace FoxEngine;
using namespace FoxEngine::Physics;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::FoxMath;

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawn = info.spawnDelay;
	mLifeTime = info.lifeTime;
	mParticles.resize(info.maxParticles);

	for (auto&p : mParticles)
	{
		p.Initialize();
	}
}

void ParticleSystem::Terminate()
{
	for (auto& p : mParticles)
	{
		p.Terminate();
	}
}

void ParticleSystem::Update(float deltaTime)
{
	if (mLifeTime > 0.0f)
	{
		mLifeTime -= deltaTime;
		mNextSpawn -= deltaTime;

		if (mNextSpawn <= 0.0f)
		{
			SpawnParticles();
		}

		for (auto& p : mParticles)
		{
			p.Update(deltaTime);
		}
	}
}

void ParticleSystem::DebugUI()
{}

void ParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.minParticlesPerEmit + (rand() % (mInfo.maxParticlesPerEmit - mInfo.minParticlesPerEmit));

	for (int i = 0; i < numParticles; i++)
	{
		SpawnParticle();
	}
	mNextSpawn = mInfo.spawnDelay;
}

void ParticleSystem::SpawnParticle()
{
	Particle& p = mParticles[mNextAvailableParticleIndex];
	mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();
}