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
    mSettingsBuffer.Initialize();

    mVertexShader.Initialize<Vertex>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void FoxEngine::Graphics::StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();

    mSettingsBuffer.Terminate();
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

    mSettingsBuffer.BindPS(3);
    mSettingsBuffer.BindVS(3);

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

    SettingsData settingsData;
    settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId != 0;
    settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId != 0;
    settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId != 0;
    settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId != 0;
    settingsData.bumpWeigh = mSettingsData.bumpWeigh;

    mSettingsBuffer.Update(settingsData);

    auto tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);
    tm->BindPS(renderObject.normalMapId, 1);
    tm->BindVS(renderObject.bumpMapId, 2);
    tm->BindPS(renderObject.specMapId, 3);

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
    if (ImGui::CollapsingHeader("StandardEffect##"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("Use Diffuse Map##", &useDiffuseMap))
        {
            mSettingsData.useDiffuseMap = (useDiffuseMap)? 1 : 0;
        }
        bool useNormalMap = mSettingsData.useNormalMap > 0;
        if (ImGui::Checkbox("Use Normal Map##", &useNormalMap))
        {
            mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;
        }
        bool useBumpMap = mSettingsData.useBumpMap > 0;
        if (ImGui::Checkbox("Use Bump Map##", &useBumpMap))
        {
            mSettingsData.useBumpMap = (useBumpMap) ? 1 : 0;
        }  
        if (useBumpMap)
        {
            ImGui::DragFloat("BumpWeight##",&mSettingsData.bumpWeigh, 0.1f, 0.0f, 2.0f );
        }
        bool useSpecMap = mSettingsData.useSpecMap > 0;
        if (ImGui::Checkbox("Use Specular Map##", &useSpecMap))
        {
            mSettingsData.useSpecMap = (useSpecMap) ? 1 : 0;
        }
    }
}
