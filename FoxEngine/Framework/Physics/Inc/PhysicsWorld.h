#pragma once

#include "PhysicsDebugDrawer.h"

namespace FoxEngine::Physics
{
	class PhysicsObject;

	class PhysicsWorld final
	{
	public:

		struct Settings 
		{
			FoxEngine::FoxMath::Vector3 gravity{ 0.0f, -9.81f, 0.0f };
			uint32_t simulationSteps = 1;
			float fixedTimeStep = 1.0f / 60.0f;
		};

		static void StaticInitialize(const Settings& settings);
		static void StaticTerminate();
		static PhysicsWorld* Get();

		PhysicsWorld() = default;
		~PhysicsWorld();

		void Initialize(const Settings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void Register(PhysicsObject* physicsObject);
		void Unregister(PhysicsObject* physicsObject);

	private:

		Settings mSettings;
		bool mRenderDebugUI = false;

		PhysicsDebugDrawer mDebugDrawer;
		//bullet objects
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btDiscreteDynamicsWorld* mDynamicWorld = nullptr;
		btSequentialImpulseConstraintSolver* mSolver = nullptr;

		//Soft body physics
		btSoftRigidDynamicsWorld* mSoftBodyWorld = nullptr;
		friend class SoftBody;
		btSoftBody* CreateSoftBody(int nodeCount);

		using PhysicsObjects = std::vector<PhysicsObject*>;
		PhysicsObjects mPhysicsObjects;
	};
}