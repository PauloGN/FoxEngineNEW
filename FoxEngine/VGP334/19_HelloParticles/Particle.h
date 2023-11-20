#pragma once

#include <FoxEngine/Inc/FoxEngine.h>

struct ParticleInfo
{
	float lifeTime = 0.0f;

	FoxEngine::Color colorStart = FoxEngine::Colors::White;
	FoxEngine::Color colorEnd = FoxEngine::Colors::White;

	FoxEngine::FoxMath::Vector3 scaleStart = FoxEngine::FoxMath::Vector3::One;
	FoxEngine::FoxMath::Vector3 scaleEnd = FoxEngine::FoxMath::Vector3::One;
};

class Particle
{
public:

	Particle() = default;

	void Initialize();
	void Terminate();

	void Activate(const ParticleInfo& info, FoxEngine::FoxMath::Vector3& pos, FoxEngine::FoxMath::Vector3& velocity);

	void Update(float DeltaTime);

	template <class Effect>
	void Render(Effect& effect)
	{
		if (mIsActive)
		{
			effect.Render(mParticle);
		}
	}
private:

	FoxEngine::Graphics::RenderObject mParticle;
	FoxEngine::Graphics::MeshPC mMesh;
	FoxEngine::Physics::RigidBody mRigidBody;
	FoxEngine::Physics::CollisionShape mShape;

	bool mIsActive = false;
	ParticleInfo mInfo;
	float mLifeTime = 0.0f;
	float mLifeTimeDuration = 0.0f;
};