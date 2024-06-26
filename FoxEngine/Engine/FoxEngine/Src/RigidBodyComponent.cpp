#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameWorld.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "PhysicsService.h"
#include "SaveUtil.h"

using namespace FoxEngine;

void RigidBodyComponent::Initialize()
{
	//
	ColliderComponent* collider = GetOwner().GetComponent<ColliderComponent>();
	TransformComponent* transform = GetOwner().GetComponent<TransformComponent>();

	ASSERT(collider != nullptr, "RigidBody: game object requires collider");
	mRigidBody.Initialize(*transform, collider->mCollisionShape, mMass);

	PhysicsService* ps = GetOwner().GetWorld().GetService<PhysicsService>();
	ps->Register(*this);
}

void RigidBodyComponent::Terminate()
{
	//
	PhysicsService* ps = GetOwner().GetWorld().GetService<PhysicsService>();
	ps->Unregister(*this);
	mRigidBody.Terminate();
}

void RigidBodyComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveFloat("Mass", mMass, doc, componentValue);
	
	//Save component name/data 
	value.AddMember("RigidBodyComponent", componentValue, doc.GetAllocator());
}

void RigidBodyComponent::Deserialize(const rapidjson::Value& value)
{
	//
	if(value.HasMember("Mass"))
	{
		mMass = value["Mass"].GetFloat();
	}
}

void RigidBodyComponent::SetPosition(const FoxEngine::FoxMath::Vector3& position)
{
	mRigidBody.SetPosition(position);
}
