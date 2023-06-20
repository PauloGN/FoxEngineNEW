#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void FoxEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
	CreateIndexBuffer(indices, indexCount);
}

void FoxEngine::Graphics::MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void FoxEngine::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case FoxEngine::Graphics::MeshBuffer::Topology::Points:

		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		break;
	case FoxEngine::Graphics::MeshBuffer::Topology::Lines:

		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		break;
	case FoxEngine::Graphics::MeshBuffer::Topology::Triangles:

		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		break;
	default:

		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

void FoxEngine::Graphics::MeshBuffer::Update(const void* vertices, uint32_t vertexCount)
{

	mVertexCount = vertexCount;
	auto context = GraphicsSystem::Get()->GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, vertexCount * mVertexSize);
	context->Unmap(mVertexBuffer, 0);

}

void FoxEngine::Graphics::MeshBuffer::Render()
{
	auto contex = GraphicsSystem::Get()->GetContext();

	contex->IASetPrimitiveTopology(mTopology);

	UINT offset = 0;
	contex->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertexSize, &offset);

	if (mIndexBuffer != nullptr)
	{
		contex->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		contex->DrawIndexed(mIndexCount, 0, 0 );
	}
	else
	{
		contex->Draw((UINT)mVertexCount, 0);
	}
}

void FoxEngine::Graphics::MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mVertexCount = vertexCount;
	mVertexSize = vertexSize;

	const bool isDynamic = (vertices == nullptr);


	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	//==========================================================
	//Create a vertex buffer

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(vertexCount * vertexSize);//how much space I need in this vertex buffer
	bufferDesc.Usage = (isDynamic)? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//is going to read as a vertex data
	bufferDesc.CPUAccessFlags = (isDynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//passing the data
	D3D11_SUBRESOURCE_DATA initiData{};
	initiData.pSysMem = vertices;

	//unising the information and the initialization to create the vertex buffer

	HRESULT hr = device->CreateBuffer(&bufferDesc, ((isDynamic)? nullptr: &initiData), &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}

void FoxEngine::Graphics::MeshBuffer::CreateIndexBuffer(const uint32_t * indices, uint32_t indexCount)
{
	mIndexCount = indexCount;

	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	//==========================================================
	//Create a vertex buffer

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(indexCount * sizeof(uint32_t));//how much space I need in this vertex buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;//is going to read as index
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//passing the data
	D3D11_SUBRESOURCE_DATA initiData{};
	initiData.pSysMem = indices;

	//unising the information and the initialization to create the vertex buffer

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initiData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
}
