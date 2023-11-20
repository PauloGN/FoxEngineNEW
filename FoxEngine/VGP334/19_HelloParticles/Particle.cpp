#include "Particle.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Physics;
using namespace FoxEngine::FoxMath;

void Particle::Initialize()
{
	mIsActive = false;
	mLifeTime = 0.0f;
	mLifeTimeDuration = 0.0f;

	mMesh = MeshBuilder::CreatePlanePC(10, 10, 0.5f);
	mParticle.meshBuffer.Initialize(nullptr, sizeof(VertexPC), mMesh.vertices.size(), mMesh.indices.data(), mMesh.indices.size());

	mShape.InitializeEmpty();
	mRigidBody.Initialize(mParticle.transform, mShape, 0.1f);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mParticle.Terminate();
	mShape.Terminate();
}

void Particle::Activate(const ParticleInfo & info, FoxEngine::FoxMath::Vector3 & pos, FoxEngine::FoxMath::Vector3 & velocity)
{
	mIsActive = true;
	mLifeTime = 0.0f;
	mLifeTimeDuration = info.lifeTime;
	mInfo = info;

	mRigidBody.SetPosition(pos);
	mRigidBody.SetVelociity(velocity);
}

void Particle::Update(float DeltaTime)
{
	if (mIsActive)
	{
		mLifeTime += DeltaTime;
		float t = Clamp(mLifeTime / mLifeTimeDuration, 0.0f, 1.0f);

		Color newColor = Lerp(mInfo.colorStart, mInfo.colorEnd, t);
		Vector3 newScale = Lerp(mInfo.scaleStart, mInfo.scaleEnd, t);
		mParticle.transform.scale = newScale;

		for (uint32_t i = 0; i < mMesh.vertices.size(); ++i)
		{
			mMesh.vertices[i].color = newColor;
		}
		mParticle.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());
	}
}
