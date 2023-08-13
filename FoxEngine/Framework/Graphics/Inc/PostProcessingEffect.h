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
			MotionBlur
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
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessBuffer mPostProcessBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		//const Texture* mTextures[4];
		std::array<const Texture*, 4> mTextures;

		Mode mMode = Mode::None;
		float mMirrorScaleX = -1.0f;
		float mMirrorScaleY = -1.0f;
		float mBlurStrength = 5.0f;
	};
}