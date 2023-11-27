#pragma once


namespace FoxEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeSphere(float radius);
		void InitializeBox(const FoxEngine::FoxMath::Vector3& halthExtents);
		void InitializeHull(const FoxEngine::FoxMath::Vector3& halthExtents, const FoxEngine::FoxMath::Vector3& origin);
		void InitializeEmpty();
		void Terminate();

	private:
		btCollisionShape* mCollisionShape = nullptr;

		friend class RigidBody;
		btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
	};
}