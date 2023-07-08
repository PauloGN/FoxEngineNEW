#include "Precompiled.h"
#include "StandardEffect.h"

#include "MeshBuffer.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace  FoxEngine;
using namespace  FoxEngine::Graphics;
using namespace  FoxEngine::FoxMath;


void FoxEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& filePath)
{
    mConstantBuffer.Initialize(sizeof(FoxMath::Matrix4));
    mVertexShader.Initialize<VertexPX>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void FoxEngine::Graphics::StandardEffect::Terminate()
{
    mConstantBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
}

void FoxEngine::Graphics::StandardEffect::Begin()
{
    ASSERT(mCamera != nullptr, "StandardEffect: no Camera set");

    mVertexShader.Bind();
    mPixelShader.Bind();

    mConstantBuffer.BindVS(0);

    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void FoxEngine::Graphics::StandardEffect::End()
{
    //Future implementation
    Texture::UnbindPS(0);
}

void FoxEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
    const Matrix4& matworld = renderObject.transform.GetMatrix4();
    const Matrix4& matView = mCamera->GetViewMatrix();
    const Matrix4& matProj = mCamera->GetProjectionMatrix();

    const Matrix4& matFinal = Transpose(matworld * matView * matProj);
    mConstantBuffer.Update(&matFinal);

    renderObject.mDiffuseTexture.BindPS(0);
    renderObject.meshBuffer.Render();
}

void FoxEngine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void FoxEngine::Graphics::StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("Standard Effect"), ImGuiTreeNodeFlags_DefaultOpen)
    {

    }
}
