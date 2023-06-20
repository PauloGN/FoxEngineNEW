#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::Texture::UnbindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

FoxEngine::Graphics::Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture -- Terminate must be called before destruction...");
}

FoxEngine::Graphics::Texture::Texture(Texture&& rhs) noexcept
	:mShaderResourceView(rhs.mShaderResourceView)
{
	rhs.mShaderResourceView = nullptr;
}

Texture& FoxEngine::Graphics::Texture::operator=(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

void FoxEngine::Graphics::Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);

	ASSERT(SUCCEEDED(hr), "Texture -- failed to load texture %ls.", fileName.c_str());
}

void FoxEngine::Graphics::Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
	ASSERT(false, "Texture -- not yet implemented");
}

void FoxEngine::Graphics::Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void FoxEngine::Graphics::Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void FoxEngine::Graphics::Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

DXGI_FORMAT FoxEngine::Graphics::Texture::GetDXGIFormat(Format format)
{

	switch (format)
	{
	case FoxEngine::Graphics::Texture::Format::RGBA_U8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case FoxEngine::Graphics::Texture::Format::RGBA_U32:	return DXGI_FORMAT_R32G32B32A32_UINT;
	}

	return DXGI_FORMAT_R8G8B8A8_UNORM;;
}
