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

	//consider to move these to AppState
	void EngineCameraController(float deltaTime);
	void EngineFPS(float deltaTime);

protected:
	
	//View port Camera/Main Camera
	Camera mCamera;
	DirectionalLight mDirectionalLight;

	//Holds-> ConstantBuffer, VertexShader, PixelShader, Sampler
	FoxEngine::Graphics::StandardEffect mStandardEffect;

	//Holds-> Transform, MeshBuffer, Texture, Material properties
	RenderObject mRenderObject;//this is the obj representation
	RenderTarget mRenderTarget;

	float mTimePassed = 0.0f;
	float mFPS = 0.0f;
	int mFrameCount = 0;
};

