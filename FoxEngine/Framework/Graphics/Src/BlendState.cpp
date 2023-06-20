#include "Precompiled.h"
#include "BlendState.h"

#include "GraphicsSystem.h"


using namespace FoxEngine::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{

		switch (mode)
		{
		case BlendState::Mode::Additive:			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaBlend:			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied:	return D3D11_BLEND_ONE;		 
		case BlendState::Mode::Opaque:				return D3D11_BLEND_ONE;
		}
		return D3D11_BLEND_ONE;
	}

	D3D11_BLEND GetDestBlend(BlendState::Mode mode)
	{

		switch (mode)
		{
		case BlendState::Mode::Additive:			return D3D11_BLEND_ONE;
		case BlendState::Mode::AlphaBlend:			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied:	return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::Opaque:				return D3D11_BLEND_ZERO;
		}
		return D3D11_BLEND_ZERO;
	}

}

void FoxEngine::Graphics::BlendState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}

FoxEngine::Graphics::BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "BlendState -- Terminate must be called first do a safe release");
}

void FoxEngine::Graphics::BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};

	desc.RenderTarget[0].BlendEnable = ( srcBlend != D3D11_BLEND_ONE ) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "BLENDSTATE -- failed to create blend state");
}

void FoxEngine::Graphics::BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void FoxEngine::Graphics::BlendState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}
