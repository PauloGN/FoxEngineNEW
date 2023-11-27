#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "BlendState.h"

namespace FoxEngine::Graphics
{
	class Camera;
	class RenderObject;

	class ParticleEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();
		void SetCamera(const Camera& camera);

	private:

		struct TransformData
		{
			FoxMath::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		BlendState mBlendState;

		const Camera* mCamera = nullptr;
	};
}