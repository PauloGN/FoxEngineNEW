#include "Precompiled.h"
#include "RigidBody.h"
#include "CollisionShape.h"
//#include "PhysicsWorld.h"

#include <Graphics/Inc/Transform.h>

using namespace FoxEngine;
using namespace FoxEngine::Physics;
using namespace FoxEngine::Graphics;

RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr && mMotionState == nullptr, "RigidBody: must be terminated");
}

void RigidBody::Initialize(FoxEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass,const float restitution)
{
	mGraphicsTransform = &graphicsTransform;
	mMass = mass;

	mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mMass, mMotionState, shape.GetCollisionShape());
	//Apply bounciness
	mRigidBody->setRestitution(restitution);

	//PhysicsWorld::Get()->Register(this);
}

void RigidBody::Terminate()
{
	//PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mMotionState);
	SafeDelete(mRigidBody);
	mGraphicsTransform = nullptr;
}

void RigidBody::SetPosition(const FoxEngine::FoxMath::Vector3 & position)
{
	if (IsDynamic())
	{
		mRigidBody->activate();
	}

	mGraphicsTransform->position = position;
	mRigidBody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));
}

void RigidBody::SetVelociity(const FoxEngine::FoxMath::Vector3 & velocity)
{
	mRigidBody->activate();
	mRigidBody->setLinearVelocity(velocity);
}

FoxEngine::FoxMath::Vector3 FoxEngine::Physics::RigidBody::GetVelociity()
{
	return mRigidBody->getLinearVelocity();
}

bool RigidBody::IsDynamic() const
{
	return mMass > 0.0f;
}

void RigidBody::Update()
{
	btTransform& worldTransform = mRigidBody->getWorldTransform();
	mGraphicsTransform->position = worldTransform.getOrigin();
	mGraphicsTransform->rotation = worldTransform.getRotation();
}