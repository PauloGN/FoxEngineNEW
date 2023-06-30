#include "Precompiled.h"
#include "SimpleEffect.h"

#include "MeshBuffer.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace  FoxEngine;
using namespace  FoxEngine::Graphics;
using namespace  FoxEngine::FoxMath;


void FoxEngine::Graphics::SimpleEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";

    //Create a Vertex shader
    mVertexShader.Initialize<VertexPX>(shaderFile);
    //Create a pixel shader
    mPixelShader.Initialize(shaderFile);
    mConstantBuffer.Initialize(sizeof(FoxMath::Matrix4));
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void FoxEngine::Graphics::SimpleEffect::Terminate()
{
    mSampler.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void FoxEngine::Graphics::SimpleEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mConstantBuffer.BindVS(0);
    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void FoxEngine::Graphics::SimpleEffect::End()
{
    //Future implementation
    Texture::UnbindPS(0);
}

void FoxEngine::Graphics::SimpleEffect::Render(const RenderObject& renderObject)
{
	Matrix4 matworld = renderObject.transform.GetMatrix4();
    Matrix4 matView = mCamera->GetViewMatrix();
    Matrix4 matProj = mCamera->GetProjectionMatrix();
    Matrix4 matFinal = matworld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    
    renderObject.mDiffuseTexture.BindPS(0);
    renderObject.meshBuffer.Render();
}

void FoxEngine::Graphics::SimpleEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}
