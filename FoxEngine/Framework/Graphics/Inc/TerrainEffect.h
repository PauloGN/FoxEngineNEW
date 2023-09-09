#pragma once

#include "ConstantBuffer.h"
#include "LightType.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace FoxEngine::Graphics
{
	class Camera;
	class Texture;
	class RenderObject;

	class TerrainEffect final 
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

	private:

		struct TransformData
		{
			FoxMath::Matrix4 world;
			FoxMath::Matrix4 wvp;
			FoxMath::Matrix4 lwvp;
			FoxMath::Vector3 viewPosition;
			float padding = 0;
		};

		struct SettingsData
		{
			int useShadowMap = 0;
			float depthBias = 0.0f;
			float padding[2] = {0};
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightingBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		LightingBuffer mLightingtBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		SettingsData mSettingsData;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}