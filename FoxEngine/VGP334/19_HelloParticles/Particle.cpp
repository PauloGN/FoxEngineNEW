#include "Particle.h"

using namespace FoxEngine::Graphics;
using namespace FoxEngine::Physics;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine;

void Particle::Initialize()
{
	isActive = false;
	time = 0.f;
	lifeTime = 0.f;

	mMesh = MeshBuilder::CreateSpherePC(10, 10, 0.5f);
	mParticle.meshBuffer.Initialize(nullptr, sizeof(VertexPC), mMesh.vertices.size(), mMesh.indices.data(), mMesh.indices.size());
	mShape.InitializeEmpty();
	mRigidbody.Initialize(mParticle.transform, mShape, 0.1f);
	Physics::PhysicsWorld::Get()->Register(&mRigidbody);
}

void Particle::Terminate()
{
	Physics::PhysicsWorld::Get()->Unregister(&mRigidbody);
	mRigidbody.Terminate();
	mShape.Terminate();
	mParticle.Terminate();
}

void Particle::Activate(const ParticleInfo& info, const FoxEngine::FoxMath::Vector3& pos, const FoxEngine::FoxMath::Vector3& vel)
{
	time = 0.f;
	lifeTime = info.lifetime;
	isActive = true;
	mInfo = info;
	mRigidbody.SetPosition(pos);
	mRigidbody.SetVelociity(vel);
}

void Particle::Update(const float deltaTime)
{
	if (isActive)
	{
		time += deltaTime;
		float t = Clamp(time / lifeTime, 0.f, 1.f);

		Color currentColor = Lerp(mInfo.colorStart, mInfo.colorEnd, t);
		mParticle.transform.scale = Lerp(mInfo.scaleStart, mInfo.scaleEnd, t);
		for (uint32_t i = 0; i < mMesh.vertices.size(); ++i)
		{
			mMesh.vertices[i].color = currentColor;
		}
		mParticle.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());
	}
}
