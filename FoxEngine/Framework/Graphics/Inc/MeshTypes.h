#pragma once
#include"VertexTypes.h"

namespace FoxEngine::Graphics
{

	template<class VertexT>
	struct MeshBase
	{
		using VertexType = VertexT;

		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;
	};

	//defined in VertexTypes.h

	using MeshP = MeshBase<VertexP>;//position
	using MeshPC = MeshBase<VertexPC>;//position and color
	using MeshPX = MeshBase<VertexPX>;//position and UV
	using Mesh = MeshBase<Vertex>;//Everything combined
}