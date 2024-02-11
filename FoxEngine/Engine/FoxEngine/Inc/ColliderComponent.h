#pragma once
#include "Component.h"

namespace  FoxEngine
{
	class ColliderComponent final : public Component
	{
	public:

		SET_TYPE_ID(ComponentId::Collider)

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;

	private:

		friend class RigidBodyComponent;
		Physics::CollisionShape mCollisionShape;
	};
}
