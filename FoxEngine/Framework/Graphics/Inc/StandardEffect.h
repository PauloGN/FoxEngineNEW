#pragma once

#include "ConstantBuffer.h"
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

		void DebugUI();

	private:

		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		const Camera* mCamera = nullptr;
	};
}
