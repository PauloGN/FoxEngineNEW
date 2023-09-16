#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace FoxEngine::Graphics
{

	class RenderObject;
	class Texture;

	class PostProcessingEffect
	{

	public:

		enum class Mode
		{
			None,
			Monochrome,
			Invert,
			Mirror,
			Blur,
			Combine2,
			MotionBlur,
			RadiantGlowEffect,
			Temperature
		};

		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Beging();
		void End();

		void Render(const RenderObject& renderObject);

		void SetTexture(const Texture* texture, uint32_t slot = 0);
		void SetMode(Mode mode);

		void DebugUI();

	private:

		struct PostProcessData
		{
			Mode mode = Mode::None;
			float param0 = { 0.0f };
			float param1 = { 0.0f };
			float param2 = { 0.0f };

			float param3 = { 0.0f };
			float param4 = { 0.0f };
			float param5 = { 0.0f };
			float param6 = { 0.0f };//padding
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessBuffer mPostProcessBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		//const Texture* mTextures[4];
		std::array<const Texture*, 4> mTextures;

		//
		Mode mMode = Mode::None;
		float mMirrorScaleX = -1.0f;
		float mMirrorScaleY = -1.0f;
		float mBlurStrength = 5.0f;
		//
		float mIntensity = 1.0f;
		float mGlowIntensity = 1.0f;
		float mBloom = 0.0f;
		float mSpreadBlur = 0.0f;
		float mSpreadGlow = 0.0f;
		float mBrightness = .0f;
	};
}