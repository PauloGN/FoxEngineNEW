#pragma once

#include <FoxEngine/inc/FoxEngine.h>

struct ParticleInfo
{
	float lifetime = 0.f;
	FoxEngine::Color colorStart = FoxEngine::Colors::White;
	FoxEngine::Color colorEnd = FoxEngine::Colors::White;
	FoxEngine::FoxMath::Vector3 scaleStart = FoxEngine::FoxMath::Vector3::One;
	FoxEngine::FoxMath::Vector3 scaleEnd = FoxEngine::FoxMath::Vector3::One;
};

class Particle
{
	FoxEngine::Graphics::RenderObject mParticle;
	FoxEngine::Graphics::MeshPC mMesh;
	FoxEngine::Physics::CollisionShape mShape;

	bool isActive = false;
	ParticleInfo mInfo;

	float time = 0.f;
	float lifeTime = 0.f;

public:
	Particle() = default;
	
	FoxEngine::Physics::RigidBody mRigidbody;
	void Initialize();
	void Terminate();

	void Activate(const ParticleInfo& info, const FoxEngine::FoxMath::Vector3& pos, const FoxEngine::FoxMath::Vector3& vel);
	void Update(const float deltaTime);

	template <class Effect>
	void Render(Effect& effect)
	{
		if (!isActive)
			return;

		effect.Render(mParticle);
	}
};