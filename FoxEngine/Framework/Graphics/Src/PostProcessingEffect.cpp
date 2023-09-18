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
	"MotionBlur",
	"RadiantGlowEffect",
	"Temperature"
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
	case Mode::None: {} break;
	case Mode::Monochrome: {} break;
	case Mode::Invert: {} break;
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
	case Mode::Combine2:{}break;
	case Mode::MotionBlur:
	{
		auto gs = GraphicsSystem::Get();
		const auto screenWidth = gs->GetBackBufferWidth();
		const auto screenHeiht = gs->GetBackBufferHeight();

		data.param0 = mBlurStrength / screenWidth;
		data.param1 = mBlurStrength / screenHeiht;
	}
		break;
	case Mode::RadiantGlowEffect:
	{
		data.param0 = mBlurStrength;
		data.param1 = mIntensity;
		data.param2 = mGlowIntensity;
		data.param3 = mBloom;
		data.param4 = mSpreadBlur;
		data.param5 = mSpreadGlow;
		data.param6 = mBrightness;
	}
		break;
	case Mode::Temperature:
	{
		data.param0 = mIntensity;
		data.param1 = mGlowIntensity;
	}
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
			mIntensity = 0.5f;
			mBlurStrength = 5.0f;
			mGlowIntensity = 0.0f;
			mMode = static_cast<Mode>(currentMode);

			if (mMode == Mode::RadiantGlowEffect) 
			{
				mBlurStrength = 10.0f;
				mIntensity = 4.88f;
				mGlowIntensity = 3.2f;
				mBloom = -0.56f;
				//	ImGui::DragFloat("Spread Blur##", &mSpreadBlur, .1f, -100.f, 100.f);
				mSpreadGlow = -0.432f;
				mBrightness = -13.5f;
			}

		}

		switch (mMode)
		{
		case Mode::None: {}break;
		case Mode::Monochrome: {}break;
		case Mode::Invert: {}break;
		case Mode::Mirror:
		{
			ImGui::DragFloat("Mirror ScaleX##", &mMirrorScaleX, 0.1f, -1.0f, 1.0f);
			ImGui::DragFloat("Mirror ScaleY##", &mMirrorScaleY, 0.1f, -1.0f, 1.0f);
		}
			break;
		case Mode::Blur:
		{
			ImGui::DragFloat("Blur Strength##", &mBlurStrength, 1.f, -10.0f, 100.0f);
		}
			break;
		case Mode::Combine2: {}break;
		case Mode::MotionBlur:
		{
			ImGui::DragFloat("Blur Strength##", &mBlurStrength, 1.f, -10.0f, 100.0f);
		}
			break;
		case Mode::RadiantGlowEffect:
		{
			ImGui::DragFloat("Blur in Bright Areas##", &mBlurStrength, .01f, -10.f, 10.f);
			ImGui::DragFloat("Blur Intensity##", &mIntensity, .01f, -100.f, 100.f);
			ImGui::DragFloat("Glow Intensity##", &mGlowIntensity, .01f, -100.f, 100.f);
			ImGui::DragFloat("Bloom##", &mBloom, .01f, -10.f, 10.f);
		//	ImGui::DragFloat("Spread Blur##", &mSpreadBlur, .1f, -100.f, 100.f);
			ImGui::DragFloat("Spread Glow##", &mSpreadGlow, .001f, -100.f, 100.f);
			ImGui::DragFloat("Brightness ##", &mBrightness, .1f, -100.f, 100.f);
		}
			break;
		case Mode::Temperature:
		{
			ImGui::DragFloat("Temperature shift##", &mIntensity, .01f, -10.f, 10.f);
			ImGui::DragFloat("Smoothness##", &mGlowIntensity, .01f, -10.f, 10.f);
		}
			break;
		default:
			break;
		}
	}
}