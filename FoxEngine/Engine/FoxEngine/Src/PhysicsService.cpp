#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace FoxEngine;
using namespace FoxEngine::Physics;

void PhysicsService::Update(float deltaTime)
{
	PhysicsWorld::Get()->Update(deltaTime);
}

void PhysicsService::DebugUI()
{
	PhysicsWorld::Get()->DebugUI();
}

void PhysicsService::Deserialize(rapidjson::Value& value)
{
	if(value.HasMember("Gravity"))
	{
		const auto& gravity = value["Gravity"].GetArray();
		const float x = gravity[0].GetFloat();
		const float y = gravity[1].GetFloat();
		const float z = gravity[2].GetFloat();
		PhysicsWorld::Get()->SetGravity({x, y, z});
	}
	if (value.HasMember("SimSteps"))
	{
		const uint32_t simSteps = value["SimSteps"].GetInt();
		PhysicsWorld::Get()->SetSimulationSteps(simSteps);
	}
	if (value.HasMember("FixedTimeStep"))
	{
		const float timeStep = value["FixedTimeStep"].GetFloat();
		PhysicsWorld::Get()->SetFixedTimeStep(timeStep);
	}
}

void PhysicsService::Register(RigidBodyComponent& rigidBodyComponent)
{
	PhysicsWorld::Get()->Register(&rigidBodyComponent.mRigidBody);
}

void PhysicsService::Unregister(RigidBodyComponent& rigidBodyComponent)
{
	PhysicsWorld::Get()->Unregister(&rigidBodyComponent.mRigidBody);
}
