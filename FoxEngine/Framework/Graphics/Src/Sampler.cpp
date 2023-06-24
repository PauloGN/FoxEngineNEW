#include "Precompiled.h"
#include "Sampler.h"
#include "GraphicsSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{

	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case FoxEngine::Graphics::Sampler::Filter::Point:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case FoxEngine::Graphics::Sampler::Filter::Linear:		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case FoxEngine::Graphics::Sampler::Filter::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
		default:
			ASSERT(false, "Filter -- Missing");
			break;
		}

		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAdressMode(Sampler::AddressMode addressMode)
	{

		switch (addressMode)
		{
		case FoxEngine::Graphics::Sampler::AddressMode::Border:	 return D3D11_TEXTURE_ADDRESS_BORDER;
		case FoxEngine::Graphics::Sampler::AddressMode::Clamp:	 return D3D11_TEXTURE_ADDRESS_CLAMP;
		case FoxEngine::Graphics::Sampler::AddressMode::Mirror:	 return D3D11_TEXTURE_ADDRESS_MIRROR;
		case FoxEngine::Graphics::Sampler::AddressMode::Wrap:	 return D3D11_TEXTURE_ADDRESS_WRAP;
		default:
			ASSERT(false, "Sampler -- Missing");
			break;
		}

		return D3D11_TEXTURE_ADDRESS_BORDER;
	}

}

FoxEngine::Graphics::Sampler::~Sampler()
{
	ASSERT(mSampler == nullptr, "Sampler -- Terminate must be called before destroying Sampler...");
}

void FoxEngine::Graphics::Sampler::Initialize(Filter filter, AddressMode addressMode)
{

	auto d3dFilter = GetFilter(filter);
	auto d3dAddressMode = GetAdressMode(addressMode);

	D3D11_SAMPLER_DESC desc{};
	desc.Filter = d3dFilter;
	desc.AddressU = d3dAddressMode;
	desc.AddressV = d3dAddressMode;
	desc.AddressW = d3dAddressMode;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateSamplerState(&desc, &mSampler);
	ASSERT(SUCCEEDED(hr),"Sampler -- Failed to create Sampler state");
}

void FoxEngine::Graphics::Sampler::Terminate()
{
	SafeRelease(mSampler);
}

void FoxEngine::Graphics::Sampler::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetSamplers(slot, 1, &mSampler);
}

void FoxEngine::Graphics::Sampler::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetSamplers(slot, 1 , &mSampler);
}
