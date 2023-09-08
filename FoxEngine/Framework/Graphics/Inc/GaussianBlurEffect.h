#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace FoxEngine::Graphics
{
	class RenderObject;

	class GaussianBlurEffect final
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUi();

		void SetSourceTexture(const Texture& texture) { mSourceTexture = &texture; }

		const Texture& GetHorizontalBlurTexture() const { return mHorizontalBlurRenderTarget; }
		const Texture& GetVerticalBlurTexture() const { return mVerticalBlurRenderTarget; }
		const Texture& GetResultTexture() const { return mVerticalBlurRenderTarget; }

		void SetBlurIterations(int iteration) {  mBlurIterations = iteration; }
		void SetBlurSaturation(float blurSaturation) {  mBlurSaturation = blurSaturation; }
		int& BlurIterations() { return mBlurIterations; }
		float& BlurSaturation() { return mBlurSaturation; }

	private:
		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;
		};

		using SettingsBuffer = FoxEngine::Graphics::TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		RenderTarget mHorizontalBlurRenderTarget;
		RenderTarget mVerticalBlurRenderTarget;

		VertexShader mVertexShader;
		PixelShader mHorizontalBlurPixelShader;
		PixelShader mVerticalBlurPixelShader;

		Sampler mSampler;

		const Texture* mSourceTexture = nullptr;

		int mBlurIterations = 1;
		float mBlurSaturation = 1.0f;
	};
}