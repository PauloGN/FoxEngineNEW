#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightType.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace FoxEngine::Graphics
{
	class RenderObject;

	class ShadowEffect 
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetdirectionalLight(const DirectionalLight& directionalLight) { mDirectionalLight = &directionalLight; };
		void SetFocus(const FoxMath::Vector3& focusPosition) { mFocusPosition = focusPosition; };
		void SetSize(float size) { mSize = size; };
		const Camera& GetLightCamera() const { return mLightCamera; };
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; };

	private:
		void UpdateLightCamera();

		struct TransformData
		{
			FoxMath::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const DirectionalLight* mDirectionalLight = nullptr;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		FoxMath::Vector3 mFocusPosition = FoxMath::Vector3::Zero;
		float mSize = 100.0f;
	};
}