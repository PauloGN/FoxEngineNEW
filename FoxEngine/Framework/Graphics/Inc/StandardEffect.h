#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "LightType.h"

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
		void SetDirectionalLight(const DirectionalLight& dirLight);

		void DebugUI();

	private:

		//HLSL data
		struct TransformData
		{
			FoxMath::Matrix4 world;
			FoxMath::Matrix4 wvp;
			FoxMath::Vector3 viewPosition;
			float padding = 0.0f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightingBuffer = TypedConstantBuffer<DirectionalLight>;

		TransformBuffer mTransformBuffer;
		LightingBuffer mLightingtBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight;
	};
}