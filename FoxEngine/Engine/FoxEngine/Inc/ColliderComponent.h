#pragma once
#include "Component.h"

namespace  FoxEngine
{

	//class TransformComponent;

	class ColliderComponent final : public Component
	{
	public:

		SET_TYPE_ID(ComponentId::Collider)

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;
		//void DebugUI() override;

	//	FoxMath::AABB GetAABB() const;

	private:

		friend class RigidBodyComponent;
		Physics::CollisionShape mCollisionShape;

		//const TransformComponent* mTransformPtr = nullptr;
		//FoxMath::AABB mAABB;
	};
}
