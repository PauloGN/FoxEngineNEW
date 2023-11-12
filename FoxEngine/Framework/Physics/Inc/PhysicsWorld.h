#pragma once

namespace FoxEngine::Physics
{
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

	private:

		Settings mSettings;
		bool mRenderDebugUI = false;

		//bullet objects
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btDiscreteDynamicsWorld* mDynamicWorld = nullptr;
		btSequentialImpulseConstraintSolver* mSolver = nullptr;
	};
}