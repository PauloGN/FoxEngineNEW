#pragma once
#include "PhysicsObject.h"


namespace FoxEngine::Graphics
{
	struct Transform;
}

namespace FoxEngine::Physics
{
	class CollisionShape;

	class RigidBody final : public PhysicsObject 
	{
	public:
		RigidBody() = default;
		~RigidBody() override;

		void Initialize(FoxEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetPosition(const FoxEngine::FoxMath::Vector3& position);
		void SetVelociity(const FoxEngine::FoxMath::Vector3& velocity);

		bool IsDynamic() const;
		void Update() override;

	private:
		btRigidBody* GetRigidBody() override { return mRigidBody; }

		btRigidBody* mRigidBody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mMass = 0.0f;

		FoxEngine::Graphics::Transform* mGraphicsTransform = nullptr;
	};
}