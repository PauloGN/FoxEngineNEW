#pragma once

#include "Service.h"

namespace FoxEngine
{
	class RigidBodyComponent;

	class PhysicsService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics)

		void Update(float deltaTime) override;
		void DebugUI() override;

		void Deserialize(rapidjson::Value& value) override;

		void Register(RigidBodyComponent& rigidBodyComponent);
		void Unregister(RigidBodyComponent& rigidBodyComponent);

		void SetEnabled(bool enabled);
	};
}
