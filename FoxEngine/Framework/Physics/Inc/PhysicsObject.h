#pragma once

//Foward declaration
class btRigidBody;

namespace FoxEngine::Physics
{
	class PhysicsObject
	{
	public:
		PhysicsObject() = default;
		virtual ~PhysicsObject() = default;
	protected:
		friend class PhysicsWorld;
		//sync the graphics with the physics
		virtual void Update() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
		virtual btSoftBody* GetSoftBody() { return nullptr; }
	};
}
