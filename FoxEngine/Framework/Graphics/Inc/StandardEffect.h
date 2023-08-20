#pragma once

#include "ConstantBuffer.h"
#include "LightType.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace  FoxEngine::Graphics
{
	class Camera;
	class RenderObject;
	class Texture;

	class StandardEffect final
	{

	public:

		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		//Begin and end the redering process
		void Begin();
		void End();

		//Get the object to be rendered
		void Render(const RenderObject& renderObject);
		//Set the camera that is going to see the object
		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& dirLight);
		void SetShadowMap(const Texture& ShadowMap);

		void DebugUI();

	private:

		//HLSL data
		struct TransformData
		{
			FoxMath::Matrix4 world;
			FoxMath::Matrix4 wvp;
			FoxMath::Matrix4 lwvp;
			FoxMath::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData 
		{
			int useDiffuseMap = 1;
			int useNormalMap = 0;
			int useBumpMap = 0;
			int useSpecMap = 0;
			int useCelShading = 0;
			int useShadowMap = 0;
			float bumpWeigh = 0.1f;
			float depthBias = 0.0f;
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