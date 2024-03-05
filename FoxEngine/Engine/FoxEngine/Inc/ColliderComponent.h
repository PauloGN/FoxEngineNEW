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

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;
		//void DebugUI() override;

	//	FoxMath::AABB GetAABB() const;

	private:

		friend class RigidBodyComponent;
		Physics::CollisionShape mCollisionShape;

		struct LoadinData
		{
			std::string shapeType{};
			FoxMath::Vector3 param0{};
			FoxMath::Vector3 param1{};
		};

		LoadinData mLoadinData;

		//const TransformComponent* mTransformPtr = nullptr;
		//FoxMath::AABB mAABB;
	};
}
