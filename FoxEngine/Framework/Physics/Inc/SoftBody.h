#pragma once

#include "PhysicsObject.h"
#include <Graphics/Inc/MeshTypes.h>

namespace FoxEngine::Physics
{
	class SoftBody final : public PhysicsObject
	{
	public:
		SoftBody() = default;
		~SoftBody();

		void Initialize(Graphics::Mesh& mesh, float mass, const std::vector<int>& fixedNodeIndices);
		void Terminate();

		void Update();
		btSoftBody* GetSoftBody() override { return mSoftBody; }

	private:
		btSoftBody* mSoftBody = nullptr;
		Graphics::Mesh* mMesh = nullptr;
	};
}