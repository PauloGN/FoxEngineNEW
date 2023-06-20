#include "Precompiled.h"
#include "ConstantBuffer.h"
#include "GraphicsSystem.h"


using namespace FoxEngine;
using namespace FoxEngine::Graphics;

FoxEngine::Graphics::ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr , "Buffer not released");
}

void FoxEngine::Graphics::ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	auto device = GraphicsSystem::Get()->GetDevice();

	HRESULT hr = device->CreateBuffer(&desc, nullptr, &mConstantBuffer);

	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer");
}

void FoxEngine::Graphics::ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void FoxEngine::Graphics::ConstantBuffer::Update(const void* data)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0,0);
}

void FoxEngine::Graphics::ConstantBuffer::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void FoxEngine::Graphics::ConstantBuffer::BindPS(uint32_t slot)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}
