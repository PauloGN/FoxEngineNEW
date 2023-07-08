#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine;
using namespace FoxEngine::FoxMath;
using namespace FoxEngine::Graphics;

class GameState : public FoxEngine::AppState
{
public:

	void Initialize()override;
	void Terminate()override;
	void Render()override;
	void Update(float deltaTime)override;
	void DebugUI() override;

	void EngineCameraController(float deltaTime);

protected:

	void RenderMesh(const Camera& camera, bool useTransform){}
	
	//View port Camera/Main Camera
	Camera mCamera;

	//Holds-> ConstantBuffer, VertexShader, PixelShader, Sampler
	FoxEngine::Graphics::StandardEffect mStandardEffect;

	//Holds-> Transform, MeshBuffer, Texture
	RenderObject mRenderObject;//this is the obj

};
