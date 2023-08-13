#include"Precompiled.h"
#include "PostProcessingEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	const char* gModeNames[] = {
	"None",
	"Monochrome",
	"Invert",
	"Mirror",
	"Blur",
	"Combine2",
	"MotionBlur"
	};
}

void FoxEngine::Graphics::PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);

	mPostProcessBuffer.Initialize();
}

void FoxEngine::Graphics::PostProcessingEffect::Terminate()
{
	mPostProcessBuffer.Terminate();
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

	PostProcessData data;
	data.mode = mMode;

	switch (mMode)
	{
	case Mode::None:
		break;
	case Mode::Monochrome:
		break;
	case Mode::Invert:
		break;
	case Mode::Mirror:
	{
		data.param0 = mMirrorScaleX;
		data.param1 = mMirrorScaleY;
	}
		break;
	case Mode::Blur:
	{
		auto gs = GraphicsSystem::Get();
		const auto screenWidth = gs->GetBackBufferWidth();
		const auto screenHeiht = gs->GetBackBufferHeight();

		data.param0 = mBlurStrength / screenWidth;
		data.param1 = mBlurStrength / screenHeiht;

	}
		break;
	case Mode::Combine2:
	{

	}
		break;
	case Mode::MotionBlur:
		break;
	default:
		break;
	}

	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);
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

void FoxEngine::Graphics::PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void FoxEngine::Graphics::PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);

		if (ImGui::Combo("Model##", &currentMode, gModeNames, static_cast<int>(std::size(gModeNames))))
		{
			mMode = static_cast<Mode>(currentMode);
		}

		switch (mMode)
		{
		case Mode::None:
			break;
		case Mode::Monochrome:
			break;
		case Mode::Invert:
			break;
		case Mode::Mirror:
			ImGui::DragFloat("Mirror ScaleX##", &mMirrorScaleX, 0.1f, -1.0f, 1.0f);
			ImGui::DragFloat("Mirror ScaleY##", &mMirrorScaleY, 0.1f, -1.0f, 1.0f);
			break;
		case Mode::Blur:
			ImGui::DragFloat("Blur Strength##", &mBlurStrength, 1.f, -10.0f, 100.0f);
			break;
		case Mode::Combine2:
			break;
		case Mode::MotionBlur:
			break;
		default:
			break;
		}
	}
}