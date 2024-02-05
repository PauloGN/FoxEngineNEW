#include "Precompiled.h"
#include "RenderService.h"

#include "GameWorld.h"
#include "CameraService.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void RenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	mShadowEffect.Initialize();
	mShadowEffect.SetdirectionalLight(mDirectionalLight);

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
}

void RenderService::Terminate()
{
	mStandardEffect.Terminate();
	mShadowEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
	const Camera& camera = mCameraService->GetMain();
	mStandardEffect.SetCamera(camera);

	for ( Entry& entry : mRenderEntries)
	{
		for (RenderObject& renderObject : entry.renderGroup)
		{
			renderObject.transform = *entry.transformComponent;
		}
	}

	mShadowEffect.Begin();
	for (const Entry& entry : mRenderEntries)
	{
		DrawrenderGroup(mShadowEffect, entry.renderGroup);
	}
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (const Entry& entry : mRenderEntries)
	{
		DrawrenderGroup(mStandardEffect, entry.renderGroup);
	}
	mStandardEffect.End();
}

void RenderService::DebugUI()
{
	ImGui::Text("FPS %f", mFPS);
	ImGui::Separator();
	ImGui::PushID("Light");
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Directional Light##", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
		{
			mDirectionalLight.direction = FoxMath::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	ImGui::PopID();
	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();
}

void RenderService::Deserialize(rapidjson::Value& value)
{
	if(value.HasMember("LightDirection"))
	{
		const auto& dir = value["LightDirection"].GetArray();
		const float x = dir[0].GetFloat();
		const float y = dir[1].GetFloat();
		const float z = dir[2].GetFloat();

		mDirectionalLight.direction = Normalize({x, y, z});
	}

	if (value.HasMember("ColorAmbient"))
	{
		const auto& color = value["ColorAmbient"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();

		mDirectionalLight.ambient = { r, g, b, a };
	}

	if (value.HasMember("ColorDiffuse"))
	{
		const auto& color = value["ColorDiffuse"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();

		mDirectionalLight.diffuse = { r, g, b, a };
	}

	if (value.HasMember("ColorSpecular"))
	{
		const auto& color = value["ColorSpecular"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();

		mDirectionalLight.specular = { r, g, b, a };
	}
}

void RenderService::Register(const ModelComponent* modelComponent)
{
	Entry& entry = mRenderEntries.emplace_back();

	const GameObject& gameObject = modelComponent->GetOwner();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();
	entry.renderGroup = CreateRenderGroup(modelComponent->GetModelId());
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{
	auto iter = std::find_if(
		mRenderEntries.begin(),
		mRenderEntries.end(),
		[&](const Entry& entry)
		{
			return entry.modelComponent == modelComponent;
		}
	);

	if (iter != mRenderEntries.end())
	{
		CleanupRenderGroup(iter->renderGroup);
		mRenderEntries.erase(iter);
	}
}
