#pragma once

#include "ConstantBuffer.h"
#include "BlendState.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace FoxEngine::Graphics
{
	class Camera;
	class RenderObject;
	
	class TexturingEffect final
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);

	private:
		using TransformBuffer = FoxEngine::Graphics::TypedConstantBuffer<FoxEngine::FoxMath::Matrix4>;

		const Camera* mCamera = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		TransformBuffer mTransformBuffer;
		Sampler mSampler;

		BlendState mBlendState;
	};
}