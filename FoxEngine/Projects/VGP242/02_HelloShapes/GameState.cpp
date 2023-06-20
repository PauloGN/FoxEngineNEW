#include "GameState.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::FoxMath;

void GameState::Initialize()
{
	//Create shapes in NDC Normalized device coordinate
	// -1 to 1 (x, y, z);
	mVertices.push_back({ {-0.5f,0.0f,0.0f}, Colors::Azure });
	mVertices.push_back({ {0.0f,0.75f,0.0f}, Colors::Green });
	mVertices.push_back({ {0.5f,0.0f,0.0f}, Colors::Orange });


	auto device = GraphicsSystem::Get()->GetDevice();
	//======================================================================================
	//Crete a vertex buffer

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size() * sizeof(Vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex data");
	//=====================================================================================+
	//
	//======================================================================================
	//Crete a Vertex Shader
	//std::filesystem::path shaderFile = L"../../../Assets/Shaders/DoNothing.fx";
	std::filesystem::path shaderFile = L"../../../Assets/Shaders/DoSomething.fx";

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		shaderFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if(errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}

	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");
	//======================================================================================
	//
	//======================================================================================
	//Crete an input layer

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	ASSERT(SUCCEEDED(hr), "Failed to create Input layout");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
	//======================================================================================
	//
	//======================================================================================
	//Crete a pixel shader
	hr = D3DCompileFromFile(
		shaderFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}

	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create Pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	mVertices.clear();
	SafeRelease(mPixelShader);
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
	SafeRelease(mVertexBuffer);
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->Draw((UINT)mVertices.size(), 0);
}