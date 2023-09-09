#include "Precompiled.h"
#include "TerrainEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void TerrainEffect::Initialize()
{
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Terrain.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mLightingtBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightingtBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TerrainEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightingtBuffer.BindPS(1);
	mLightingtBuffer.BindVS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindPS(3);
	mSettingsBuffer.BindVS(3);

	mSampler.BindPS(0);
	mSampler.BindVS(0);
}

void TerrainEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(2);
	}
}

void TerrainEffect::Render(const RenderObject & renderObject)
{
	ASSERT(mCamera != nullptr, "TerrainEffect: no Camera specified");
	ASSERT(mDirectionalLight != nullptr, "TerrainEffect: no Camera specified");

	const auto& matworld = renderObject.transform.GetMatrix4();
	auto matView = mCamera->GetViewMatrix();
	auto matProj = mCamera->GetProjectionMatrix();

	TransformData transformData;
	transformData.world = FoxMath::Transpose(matworld);
	transformData.wvp = FoxMath::Transpose(matworld * matView * matProj);

	if (mShadowMap != nullptr && mLightCamera != nullptr)
	{
		matView = mLightCamera->GetViewMatrix();
		matProj = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = FoxMath::Transpose(matworld * matView * matProj);
	}

	transformData.viewPosition = mCamera->GetPosition();

	SettingsData settingsData;
	if (mSettingsData.useShadowMap > 0 && mShadowMap != nullptr)
	{
		mShadowMap->BindPS(2);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mSettingsData.depthBias;
	}

	mTransformBuffer.Update(transformData);
	mSettingsBuffer.Update(settingsData);
	mMaterialBuffer.Update(renderObject.material);
	mLightingtBuffer.Update(*mDirectionalLight);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMapId, 1);

	renderObject.meshBuffer.Render();
}

void TerrainEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Terrain##Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("Use Shadow Map##Terrain", &useShadowMap))
		{
			mSettingsData.useShadowMap = (useShadowMap) ? 1 : 0;
		}
		ImGui::DragFloat("Depth Bias##Terrain", &mSettingsData.depthBias, 0.0000001f, 0.f, 1.f, "%.6f");
	}
}

void TerrainEffect::SetCamera(const Camera & camera)
{
	mCamera = &camera;
}

void TerrainEffect::SetLightCamera(const Camera & camera)
{
	mLightCamera = &camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight & directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void TerrainEffect::SetShadowMap(const Texture & shadowMap)
{
	mShadowMap = &shadowMap;
}