#include "ParticleSystem.h"

using namespace FoxEngine::Graphics;
using namespace FoxEngine::Physics;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine;

void ParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.minParticlesPerEmit + (rand() % mInfo.maxParticlesPerEmit - mInfo.minParticlesPerEmit);
	for (uint32_t i = 0; i < numParticles; ++i)
	{
		SpawnParticle();
	}
	float randFloat = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	nextSpawn = mInfo.minTimeBetweenParticles + ((mInfo.maxTimeBetweenParticles - mInfo.minTimeBetweenParticles) * randFloat);
}

void ParticleSystem::SpawnParticle()
{
	Particle& p = mParticles[nextAvailableParticleIndex];
	nextAvailableParticleIndex = (nextAvailableParticleIndex + 1) % mParticles.size();

	float randFloat = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	float randAngle = mInfo.minSpawnAngle + ((mInfo.maxSpawnAngle - mInfo.minSpawnAngle) * randFloat);

	Vector3 rotAxis = (Dot(mInfo.spawnDirection, Vector3::YAxis) >= 0.99f) ? Vector3::XAxis : Vector3::YAxis;
	rotAxis = Normalize(Cross(rotAxis, mInfo.spawnDirection));
	Matrix4 matRot = Matrix4::RotationAxis(rotAxis, randAngle);
	Vector3 spawnDir = TransformNormal(mInfo.spawnDirection, matRot);

	randFloat = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);
	p.Activate(mInfo.particleInfo, mInfo.spawnPosition, spawnDir * speed);

}

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	nextAvailableParticleIndex = 0;
	nextSpawn = info.spawnDelay;
	mLifeTime = info.lifeTime;
	mParticles.resize(info.maxParticles);
	for (auto& p : mParticles)
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

void ParticleSystem::Update(const float deltaTime)
{
	if (mLifeTime > 0.f)
	{
		mLifeTime -= deltaTime;
		nextSpawn -= deltaTime;
		if (nextSpawn <= 0.f)
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
{
	if (ImGui::CollapsingHeader("Particles"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragInt("Max Particles", &mInfo.maxParticles);
		ImGui::DragFloat3("Spawn Position##Part", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("Spawn Direction##Part", &mInfo.spawnDirection.x))
		{
			mInfo.spawnDirection = FoxEngine::FoxMath::Normalize(mInfo.spawnDirection);
		}
		ImGui::DragInt("Min Particles per Emit", &mInfo.minParticlesPerEmit);
		ImGui::DragInt("Max Particles per Emit", &mInfo.maxParticlesPerEmit, 1, mInfo.minParticlesPerEmit +1, 50);
		ImGui::DragFloat("Min Time Between Particles", &mInfo.minTimeBetweenParticles, 0.1f);
		ImGui::DragFloat("Max Time Between Particles", &mInfo.maxTimeBetweenParticles, 0.1f, mInfo.minTimeBetweenParticles, 10.0f);
		ImGui::DragFloat("Min Spawn Angle", &mInfo.minSpawnAngle, 0.01f);
		ImGui::DragFloat("Max Spawn Angle", &mInfo.maxSpawnAngle, 0.01f, mInfo.minSpawnAngle, 3.0f);
		ImGui::DragFloat("Min Speed##Part", &mInfo.minSpeed);
		ImGui::DragFloat("Max Speed##Part", &mInfo.maxSpeed, 1.0f, mInfo.minSpeed, 100.0f);
		ImGui::DragFloat("Particle Lifetime", &mInfo.particleInfo.lifetime);
		ImGui::ColorEdit4("Start Color##Part", &mInfo.particleInfo.colorStart.r);
		ImGui::ColorEdit4("End Color##Part", &mInfo.particleInfo.colorEnd.r);
		ImGui::DragFloat3("Scale Start##Part", &mInfo.particleInfo.scaleStart.x);
		ImGui::DragFloat3("Scale End##Part", &mInfo.particleInfo.scaleEnd.x);
	}
}
