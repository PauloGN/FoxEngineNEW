#pragma once

#include "MeshTypes.h"

namespace FoxEngine::Graphics
{
	class Terrain final 
	{
	public:
		void Initialize(const std::filesystem::path& fileName, float heightScale);
		float GetHeight(const FoxMath::Vector3& position) const;

		Mesh mesh;
		uint32_t mRows = 0;
		uint32_t mColumns = 0;
	};
}