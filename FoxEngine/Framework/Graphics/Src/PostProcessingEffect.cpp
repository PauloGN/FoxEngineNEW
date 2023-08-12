#include"Precompiled.h"
#include "PostProcessingEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;


void FoxEngine::Graphics::PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void FoxEngine::Graphics::PostProcessingEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void FoxEngine::Graphics::PostProcessingEffect::Beging()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			mTextures[i]->BindPS(i);
		}
	}
}

void FoxEngine::Graphics::PostProcessingEffect::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void FoxEngine::Graphics::PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void FoxEngine::Graphics::PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "PostProcessing: Invalid slot index");
	mTextures[slot] = texture;
}

void FoxEngine::Graphics::PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect##", ImGuiTreeNodeFlags_DefaultOpen))
	{



	}
}