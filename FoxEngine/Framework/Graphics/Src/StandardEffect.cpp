#include "Precompiled.h"
#include "StandardEffect.h"

#include "MeshBuffer.h"
#include "Camera.h"
#include "RenderObject.h"
#include "TextureManager.h"
#include "VertexTypes.h"

using namespace  FoxEngine;
using namespace  FoxEngine::Graphics;
using namespace  FoxEngine::FoxMath;

void FoxEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& filePath)
{
    //HLSL size of data init
    mTransformBuffer.Initialize();
    mLightingtBuffer.Initialize();
    mMaterialBuffer.Initialize();

    mVertexShader.Initialize<Vertex>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void FoxEngine::Graphics::StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();

    mMaterialBuffer.Terminate();
    mLightingtBuffer.Terminate();
    mTransformBuffer.Terminate();
}

void FoxEngine::Graphics::StandardEffect::Begin()
{
    ASSERT(mCamera != nullptr, "StandardEffect: No Camera set!");

    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);

    mLightingtBuffer.BindVS(1);
    mLightingtBuffer.BindPS(1);

    mMaterialBuffer.BindPS(2);

    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void FoxEngine::Graphics::StandardEffect::End()
{
    //Future implementation
   // Texture::UnbindPS(0);
}

void FoxEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
    const Matrix4& matworld = renderObject.transform.GetMatrix4();
    const Matrix4& matView = mCamera->GetViewMatrix();
    const Matrix4& matProj = mCamera->GetProjectionMatrix();

    TransformData transformData;
    transformData.world = Transpose(matworld);
    transformData.wvp = Transpose(matworld * matView * matProj);
    transformData.viewPosition = mCamera->GetPosition();

    mTransformBuffer.Update(transformData);
    mLightingtBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    auto tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);
    tm->BindPS(renderObject.normalMap, 1);

    renderObject.meshBuffer.Render();
}

void FoxEngine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void FoxEngine::Graphics::StandardEffect::SetDirectionalLight(const DirectionalLight& dirLight)
{
    mDirectionalLight = &dirLight;
}

void FoxEngine::Graphics::StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("Standard##Effect"), ImGuiTreeNodeFlags_DefaultOpen)
    {

    }
}
