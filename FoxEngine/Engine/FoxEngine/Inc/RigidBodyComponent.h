#pragma once

#include "Component.h"

namespace FoxEngine
{
	class RigidBodyComponent final: public Component
	{
	public:

		SET_TYPE_ID(ComponentId::RigidBody)

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;

		void SetPosition(const FoxEngine::FoxMath::Vector3& position);

	private:

		friend class PhysicsService;

		Physics::RigidBody mRigidBody;
		float mMass = 1.0f;
	};
}
