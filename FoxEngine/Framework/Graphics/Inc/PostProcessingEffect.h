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
			MonoChrome,
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
		void SetMode() {}

		void DebugUI();

	private:

		struct PostProcessData
		{
			Mode mMode = Mode::None;
			float params[3] = { 0.0f };
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessBuffer mPostProcessBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		//const Texture* mTextures[4];
		std::array<const Texture*, 4> mTextures;
	};
}