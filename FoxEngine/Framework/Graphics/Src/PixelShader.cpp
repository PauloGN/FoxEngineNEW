#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicsSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::PixelShader::Initialize(const std::filesystem::path& filePath, const char* entryPoint)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//==========================================================
	//Create a pixel shader

	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);

	if (errorBlob && errorBlob->GetBufferPointer())
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile Pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);

	ASSERT(SUCCEEDED(hr), "Failed to create Pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void FoxEngine::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void FoxEngine::Graphics::PixelShader::Bind()
{
	auto contex = GraphicsSystem::Get()->GetContext();
	contex->PSSetShader(mPixelShader, nullptr, 0);
}
